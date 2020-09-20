// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maintenance/InteractionInterface.h"
#include "Maintenance/Power/PowerConsumerBase.h"
#include "DoorBase.generated.h"

UCLASS()
class MAINTENANCE_API ADoorBase : public APowerConsumerBase, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	bool bOpen = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key,SaveGame)
	int NeededKeyId = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Key,SaveGame)
	bool bNeedsKey = false;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Toggle();

	void Toggle_Implementation(){}

	
	virtual void Interact_Implementation(AActor* Interactor, UActorComponent* InteractedComponent) override;

	virtual void UseKeys_Implementation(AActor* interactor,const ::TArray<FKeyInfo> &Keys) override;
};
