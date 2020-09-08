// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerConsumerBase.h"
#include "GameFramework/Actor.h"
#include "Maintenance/InteractionInterface.h"

#include "PowerSourceBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivatedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeactivatedDelegate);

UCLASS()
class MAINTENANCE_API APowerSourceBase : public AActor,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerSourceBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnActivatedDelegate OnActivated;

	UPROPERTY(BlueprintAssignable)
	FOnDeactivatedDelegate OnDeactivated;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=State)
	bool bActivated = false;

	//if false actor can only be activated
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=State)
	bool bToggleable = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<APowerConsumerBase*> Consumers;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Sound)
	USoundBase*ActivationSound;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Sound)
	USoundBase*DeactivationSound;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key)
	int NeededKeyId = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key)
	bool bNeedsKey = false;

	virtual  void Interact_Implementation(AActor* Interactor, UActorComponent* InteractedComponent) override;

	virtual  void UseItem_Implementation(AActor* Item, UActorComponent* InteractedComponent) override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnPowerStateChanged();

	void OnPowerStateChanged_Implementation();
	
};
