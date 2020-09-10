// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldableActorComponent.h"

// Sets default values for this component's properties
UHoldableActorComponent::UHoldableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHoldableActorComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr)
	{
		TArray<UActorComponent*> comps;
		GetOwner()->GetComponents(UStaticMeshComponent::StaticClass(), comps);
		if (comps.Num() > 0) //there is at least one component on actor - this component
		{
			for (int i = 0; i < comps.Num(); i++)
			{
				if (comps[i]->ComponentTags.Find(PhysComponentsTag) != INDEX_NONE)
				{
					StaticMeshPhysicsComponents.Add(Cast<UStaticMeshComponent>(comps[i]));
				}
			}
		}
		TArray<UActorComponent*> skelComps;

		GetOwner()->GetComponents(USkeletalMeshComponent::StaticClass(), skelComps);
		if (skelComps.Num() > 0) //there is at least one component on actor - this component
		{
			for (int i = 0; i < skelComps.Num(); i++)
			{
				if (skelComps[i]->ComponentTags.Find(PhysComponentsTag) != INDEX_NONE)
				{
					SkeletalMeshPhysicsComponents.Add(Cast<USkeletalMeshComponent>(skelComps[i]));
				}
			}
		}
	}
}

void UHoldableActorComponent::ApplyImpulseToComponents()
{

	if (SkeletalMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < SkeletalMeshPhysicsComponents.Num(); i++)
		{
			SkeletalMeshPhysicsComponents[i]->AddImpulse(_impulse);
		}
	}

	if (StaticMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < StaticMeshPhysicsComponents.Num(); i++)
		{
			StaticMeshPhysicsComponents[i]->AddImpulse(_impulse);
		}
	}
	ChildOnThrown(_impulse);
	
	_impulse= FVector::ZeroVector;
}


void UHoldableActorComponent::BeDropped()
{
	if (SkeletalMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < SkeletalMeshPhysicsComponents.Num(); i++)
		{
			SkeletalMeshPhysicsComponents[i]->SetSimulatePhysics(true);
		}
	}

	if (StaticMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < StaticMeshPhysicsComponents.Num(); i++)
		{
			StaticMeshPhysicsComponents[i]->SetSimulatePhysics(true);
		}
	}
	
	OnDropped.Broadcast();
	
	ChildOnBeDropped();
}

void UHoldableActorComponent::BeThrown(FVector impulse)
{
	_impulse = impulse;
	GetWorld()->GetTimerManager().SetTimer(ImpulseApplyTimerHandle, this, &UHoldableActorComponent::ApplyImpulseToComponents, 0.01f);
}

void UHoldableActorComponent::BePickedUp()
{
	if (SkeletalMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < SkeletalMeshPhysicsComponents.Num(); i++)
		{
			SkeletalMeshPhysicsComponents[i]->SetSimulatePhysics(false);
		}
	}

	if (StaticMeshPhysicsComponents.Num() > 0)
	{
		for (int i = 0; i < StaticMeshPhysicsComponents.Num(); i++)
		{
			StaticMeshPhysicsComponents[i]->SetSimulatePhysics(false);
		}
	}
	
	OnPickedUp.Broadcast();
	
	ChildOnBePickedup();
}

// Called every frame
void UHoldableActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

