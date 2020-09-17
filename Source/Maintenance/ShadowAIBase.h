// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MaintenanceCharacter.h"

#include "ShadowAIBase.generated.h"

/**
 * 
 */
UCLASS()
class MAINTENANCE_API AShadowAIBase : public AAIController
{
	GENERATED_BODY()
	
	public:
	AShadowAIBase();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<AActor*> PatrolPoints;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector LastNoiseLocation;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector LastSeenLocation;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AMaintenanceCharacter * Target;
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdatePerceivedActors(TArray<AActor*>SeenActors,TArray<AActor*>HeardActors);
	
	//returns assigned point
	UFUNCTION(BlueprintCallable)
	virtual AActor* AssignNewPatrolPoint();

	UFUNCTION(BlueprintPure)
	AActor* GetCurrentPatrolPoint();
};
