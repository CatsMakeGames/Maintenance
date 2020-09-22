// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"


#include "KeyComponent.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorBase::Interact_Implementation(AActor* Interactor, UActorComponent* InteractedComponent)
{
	if(bHasPower && !bNeedsKey)//bHasPower is used here as "Locked"
	{
		Toggle();
	}
	else
	{
		PlayDoorLockedEffects();
	}
}

void ADoorBase::UseKeys_Implementation(AActor* interactor,const ::TArray<FKeyInfo> &Keys)
{
	for (int i = 0; i < Keys.Num(); i++)
	{
		if(Keys[i].KeyId == NeededKeyId)
		{
			bNeedsKey = false;
			if(Keys[i].OneTimeUse)
			{
				if (interactor->Implements<UInteractionInterface>() || (Cast<IInteractionInterface>(interactor) != nullptr))
				{
					IInteractionInterface::Execute_RemoveKey(interactor,Keys[i].Name);
				}
			}
			OnUnlocked();
		}
	}
}

