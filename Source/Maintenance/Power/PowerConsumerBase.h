// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerConsumerBase.generated.h"

UCLASS()
class MAINTENANCE_API APowerConsumerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerConsumerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=State)
	bool bHasPower = false;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnPowerStateChanged();

	void OnPowerStateChanged_Implementation();
};

