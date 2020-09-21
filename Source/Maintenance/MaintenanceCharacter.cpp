// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaintenanceCharacter.h"

#include "DrawDebugHelpers.h"
#include "MaintenanceProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "InteractionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Kismet/KismetMathLibrary.h"
#include "Pickup/HoldableActorComponent.h"
#include "Tools/FlashlightBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMaintenanceCharacter

AMaintenanceCharacter::AMaintenanceCharacter()
{

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	ActorHoldingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ActorHoldingPosition"));
	ActorHoldingPosition->SetupAttachment(FirstPersonCameraComponent);

	ActorDroppingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ActorDroppingPosition"));
	ActorDroppingPosition->SetupAttachment(FirstPersonCameraComponent);
	
}

void AMaintenanceCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void AMaintenanceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMaintenanceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMaintenanceCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMaintenanceCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMaintenanceCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMaintenanceCharacter::PlayerUseItem);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMaintenanceCharacter::PlayerInteract);

	PlayerInputComponent->BindAction("Throw", IE_Released, this, &AMaintenanceCharacter::Throw);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AMaintenanceCharacter::BeginThrow);
}

void AMaintenanceCharacter::ChangeToThrow()
{
	bIsThrowing = true;
}

void AMaintenanceCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMaintenanceCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMaintenanceCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMaintenanceCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMaintenanceCharacter::CanBeSeen_Implementation()
{
	return !bIsInSafeZone;
}

void AMaintenanceCharacter::RemoveKey_Implementation(const FString& keyName)
{
	if (Keys.Num() > 0)
	{
		for (int i = 0; i < Keys.Num(); i++)
		{
			if(Keys[i].Name == keyName)
			{
				Keys.RemoveAt(i);
				return;
			}
		}
	}
}

bool AMaintenanceCharacter::CanBeSeenByThisShadow_Implementation(AActor* Shadow, float SightRadius)
{
	if(!bIsInSafeZone)
	{
		AFlashlightBase* light = Cast<AFlashlightBase>(GetToolByClass(AFlashlightBase::StaticClass()));
		if (light != nullptr)
		{
			if (light->bActivated)
			{				
				return FVector::Distance(GetActorLocation(), Shadow->GetActorLocation()) < SightRadius / 2;			
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

AToolBase* AMaintenanceCharacter::GetToolByClass(TSubclassOf<AToolBase> Class)
{
	if (Tools.Num() > 0)
	{
		for (int i = 0; i < Tools.Num(); i++)
		{
			if (Tools[i]->GetClass() == Class) { Tools[i]; }
		}
	}
	return nullptr;
}

bool AMaintenanceCharacter::SelectTool(FString name)
{
	if (Tools.Num() > 0)
	{
		if (CurrentlyHeldActor != nullptr)
		{
			CurrentlyHeldActor->SetActorHiddenInGame(true);
		}
		for (int i = 0; i < Tools.Num(); i++)
		{
			if (Tools[i]->ToolName == name)
			{
				Tools[i]->SetActorHiddenInGame(false);
				CurrentlySelectedToolId = i;
				return true;
			}
			else
			{
				Tools[i]->SetActorHiddenInGame(true);
			}
		}
	}
	return false;
}

bool AMaintenanceCharacter::AddTool(AToolBase* tool)
{
	if (Tools.Num() > 0)
	{
		for (int i = 0; i < Tools.Num(); i++)
		{
			if(Tools[i]->ToolName == tool->ToolName)
			{
				return false;
			}
		}
	}

	tool->SetActorEnableCollision(false);
	tool->DisableComponentsSimulatePhysics();
	tool->SetActorLocation(ActorHoldingPosition->GetComponentLocation());
	tool->AttachToComponent(ActorHoldingPosition,  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	tool->SetActorHiddenInGame(true);
	Tools.Add(tool);
	
	return true;
}

void AMaintenanceCharacter::ReattachTools()
{
	if (Tools.Num() > 0)
	{
		for (int i = 0; i < Tools.Num(); i++)
		{
			Tools[i]->SetActorEnableCollision(false);
			Tools[i]->DisableComponentsSimulatePhysics();
			Tools[i]->SetActorLocation(ActorHoldingPosition->GetComponentLocation());
			Tools[i]->AttachToComponent(ActorHoldingPosition, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Tools[i]->SetActorHiddenInGame(i == CurrentlySelectedToolId ? false : true);
		}
	}
}

void AMaintenanceCharacter::SelectHand()
{
	CurrentlySelectedToolId = -1;
	if (CurrentlyHeldActor != nullptr)
	{
		CurrentlyHeldActor->SetActorHiddenInGame(false);
	}
	if (Tools.Num() > 0)
	{
		for (int i = 0; i < Tools.Num(); i++)
		{
			Tools[i]->SetActorHiddenInGame(true);
		}
	}
}

bool AMaintenanceCharacter::HasTool(FString name)
{
	if (Tools.Num() > 0)
	{
		for (int i = 0; i < Tools.Num(); i++)
		{
			if (Tools[i]->ToolName == name) { return true; }
		}
	}
	return false;
}

void AMaintenanceCharacter::PlayerInteract()
{
	//do line trace
	FHitResult hit;
	FVector start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector end = UKismetMathLibrary::GetForwardVector(GetFirstPersonCameraComponent()->GetComponentRotation()) *
        InteractionLength;

	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(hit, start, start + end, ECollisionChannel::ECC_Camera, params);

	if (hit.bBlockingHit)
	{
		if (hit.GetActor() != nullptr)
		{
			if(Cast<AToolBase>(hit.GetActor()) != nullptr)
			{
				AddTool(Cast<AToolBase>(hit.GetActor()));
			}
			else if(hit.GetActor()->FindComponentByClass(UKeyComponent::StaticClass())!= nullptr)
			{
				UKeyComponent*key = Cast<UKeyComponent>(hit.GetActor()->FindComponentByClass(UKeyComponent::StaticClass()));
				if(key->bCanBePickedUp)
				{
					key->bKeyWasPickedUp = true;
					Keys.Add(key->KeyInfo);
					hit.GetActor()->Destroy();
				}
			}
			else if (hit.GetActor()->FindComponentByClass(UHoldableActorComponent::StaticClass()) != nullptr && CurrentlyHeldActor == nullptr)
			{
				UHoldableActorComponent* comp = Cast<UHoldableActorComponent>(
                    hit.GetActor()->FindComponentByClass(UHoldableActorComponent::StaticClass()));
				if (comp != nullptr)
				{
					if(comp->bCanBePickedUp)
					{
						CurrentlyHeldActor = hit.GetActor();
						CurrentlyHeldActor->SetActorEnableCollision(false);
						CurrentlyHeldActor->DisableComponentsSimulatePhysics();
						CurrentlyHeldActor->SetActorLocation(ActorHoldingPosition->GetComponentLocation());
						CurrentlyHeldActor->AttachToComponent(ActorHoldingPosition,  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						comp->BePickedUp();
					}
				}
			}

			if(hit.GetActor()->IsValidLowLevel())
			{
				if ((hit.GetActor()->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(hit.GetActor()) != nullptr)))
				{
					IInteractionInterface::Execute_Interact(hit.GetActor(), this, hit.GetComponent());

					if (Keys.Num() > 0)
					{
						IInteractionInterface::Execute_UseKeys(hit.GetActor(), this, Keys);
					}
				}
			}
		}
	}
}



void AMaintenanceCharacter::PlayerUseItem()
{
	if(CurrentlySelectedToolId == -1)
	{
		if(CurrentlyHeldActor != nullptr)
		{
			//do line trace
			FHitResult hit;
			FVector start = GetFirstPersonCameraComponent()->GetComponentLocation();
			FVector end = UKismetMathLibrary::GetForwardVector(GetFirstPersonCameraComponent()->GetComponentRotation()) *
                InteractionLength;

			FCollisionQueryParams params = FCollisionQueryParams();
			params.AddIgnoredActor(this);

			GetWorld()->LineTraceSingleByChannel(hit, start, start + end, ECollisionChannel::ECC_Camera, params);

			if (hit.bBlockingHit)
			{
				if (hit.GetActor() != nullptr)
				{
					if (hit.GetActor()->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(hit.GetActor()) != nullptr))
					{
						IInteractionInterface::Execute_UseItem(hit.GetActor(),CurrentlyHeldActor,hit.GetComponent()); // for stuff like keys
					}
				}
			}

			if (CurrentlyHeldActor->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(CurrentlyHeldActor) != nullptr))
			{
				IInteractionInterface::Execute_UseItemInHand(CurrentlyHeldActor,this);//for stuff like flashlight
			}
		}
	}
	else if(Tools.IsValidIndex(CurrentlySelectedToolId))
	{
		
		if (Tools[CurrentlySelectedToolId]->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(Tools[CurrentlySelectedToolId]) != nullptr))
		{
			IInteractionInterface::Execute_UseItemInHand(Tools[CurrentlySelectedToolId],this);//for stuff like flashlight
		}
	}
}

void AMaintenanceCharacter::GetCaught_Implementation()
{
	if(GetController()!=nullptr)
	{
		if(Cast<APlayerController>(GetController()))
		{
			DisableInput(Cast<APlayerController>(GetController()));
		}
	}
}

void AMaintenanceCharacter::Throw()
{
	if(CurrentlyHeldActor != nullptr)
	{
		UHoldableActorComponent* comp = Cast<UHoldableActorComponent>(CurrentlyHeldActor->FindComponentByClass(UHoldableActorComponent::StaticClass()));
		comp->BeDropped();
		
		CurrentlyHeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentlyHeldActor->SetActorEnableCollision(true);
		CurrentlyHeldActor->SetActorLocation(ActorDroppingPosition->GetComponentLocation());
		if(bIsThrowing)
		{
			bIsThrowing = false;
			comp->BeThrown(GetFirstPersonCameraComponent()->GetForwardVector()* ThrowStrength);		
		}
		
		GetWorldTimerManager().ClearTimer(ThrowTimerHandle);
		
		CurrentlyHeldActor = nullptr;
	}
}

void AMaintenanceCharacter::ForcePickupActor(AActor* actor)
{
	UHoldableActorComponent* comp = Cast<UHoldableActorComponent>(actor->FindComponentByClass(UHoldableActorComponent::StaticClass()));
	if (comp != nullptr)
	{
		if(comp->bCanBePickedUp)
		{
			CurrentlyHeldActor->SetActorEnableCollision(false);
			CurrentlyHeldActor->DisableComponentsSimulatePhysics();
			CurrentlyHeldActor->SetActorLocation(ActorHoldingPosition->GetComponentLocation());
			CurrentlyHeldActor->SetActorRotation(FRotator::ZeroRotator);
			CurrentlyHeldActor->AttachToComponent(ActorHoldingPosition,  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			comp->BePickedUp();
		}
	}
}

void AMaintenanceCharacter::BeginThrow()
{
	GetWorldTimerManager().SetTimer(ThrowTimerHandle,this,&AMaintenanceCharacter::ChangeToThrow,ThrowTime,false);
}
