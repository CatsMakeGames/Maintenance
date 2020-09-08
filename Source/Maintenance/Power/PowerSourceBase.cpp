// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerSourceBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APowerSourceBase::APowerSourceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerSourceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerSourceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerSourceBase::Interact_Implementation(AActor* Interactor, UActorComponent* InteractedComponent)
{
	if(!bActivated)
	{
		bActivated = true;
		OnPowerStateChanged();
		
		if(ActivationSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),ActivationSound,GetActorLocation(),GetActorRotation());
		}
	}
	else if(bToggleable)
	{
		bActivated = false;
		OnPowerStateChanged();

		if(DeactivationSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),DeactivationSound,GetActorLocation(),GetActorRotation());
		}
	}
}

void APowerSourceBase::OnPowerStateChanged_Implementation()
{
	if(Consumers.Num() > 0)
	{
		for (int i = 0; i < Consumers.Num(); i++)
		{
			Consumers[i]->bHasPower = bActivated;
			Consumers[i]->OnPowerStateChanged();
		}
	}
	
	if(bActivated)
	{
		OnActivated.Broadcast();
	}
	else
	{
		OnDeactivated.Broadcast();
	}
}

