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
}

void ADoorBase::UseItem_Implementation(AActor* Item, UActorComponent* InteractedComponent)
{
	UActorComponent *keyComp = Item->FindComponentByClass(UKeyComponent::StaticClass());

	if (keyComp != nullptr)
	{
		if (Cast<UKeyComponent>(keyComp)->KeyId == NeededKeyId)
		{
			bNeedsKey = false;
		}
	}
}

