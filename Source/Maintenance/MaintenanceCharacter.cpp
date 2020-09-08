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
	ActorHoldingPosition->SetupAttachment(RootComponent);

	ActorDroppingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ActorDroppingPosition"));
	ActorDroppingPosition->SetupAttachment(RootComponent);
	
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

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMaintenanceCharacter::Pickup);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMaintenanceCharacter::Interact);

	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AMaintenanceCharacter::Throw);
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

void AMaintenanceCharacter::Interact()
{
	//do line trace
	FHitResult hit;
	FVector start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector end = UKismetMathLibrary::GetForwardVector(GetFirstPersonCameraComponent()->GetComponentRotation()) *
        InteractionLenght;

	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(hit, start, start + end, ECollisionChannel::ECC_Camera, params);

	if (hit.bBlockingHit)
	{
		if (hit.GetActor() != nullptr)
		{
			if (hit.GetActor()->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(hit.GetActor()) != nullptr))
			{
				IInteractionInterface::Execute_Interact(hit.GetActor(),this,hit.GetComponent());
			}
		}
	}
}

void AMaintenanceCharacter::Pickup()
{
	if(CurrentlyHeldActor == nullptr)
	{
		//do line trace
		FHitResult hit;
		FVector start = GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector end = UKismetMathLibrary::GetForwardVector(GetFirstPersonCameraComponent()->GetComponentRotation()) *
			InteractionLenght;

		FCollisionQueryParams params = FCollisionQueryParams();
		params.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(hit, start, start + end, ECollisionChannel::ECC_Camera, params);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor() != nullptr)
			{
				if (hit.GetActor()->FindComponentByClass(UHoldableActorComponent::StaticClass()) != nullptr)
				{
					UHoldableActorComponent* comp = Cast<UHoldableActorComponent>(
						hit.GetActor()->FindComponentByClass(UHoldableActorComponent::StaticClass()));
					if (comp != nullptr)
					{
						CurrentlyHeldActor = hit.GetActor();
						CurrentlyHeldActor->SetActorEnableCollision(false);
						CurrentlyHeldActor->DisableComponentsSimulatePhysics();
						CurrentlyHeldActor->SetActorLocation(ActorHoldingPosition->GetComponentLocation());
						CurrentlyHeldActor->AttachToComponent(ActorHoldingPosition,
						                                      FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						comp->BePickedUp();
					}
				}
			}
		}
	}
	else
	{
		UHoldableActorComponent* comp = Cast<UHoldableActorComponent>(CurrentlyHeldActor->FindComponentByClass(UHoldableActorComponent::StaticClass()));
		comp->BeDropped();
		
		CurrentlyHeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentlyHeldActor->SetActorEnableCollision(true);
		CurrentlyHeldActor->SetActorLocation(ActorDroppingPosition->GetComponentLocation());
		CurrentlyHeldActor = nullptr;
	}
}

void AMaintenanceCharacter::Throw()
{
	
}
