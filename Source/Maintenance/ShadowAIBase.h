// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MaintenanceCharacter.h"
#include "AI/AIInterface.h"


#include "ShadowAIBase.generated.h"

/**
 * 
 */
UCLASS()
class MAINTENANCE_API AShadowAIBase : public AAIController, public IAIInterface
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

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	float GetSightRadius();

	float GetSightRadius_Implementation(){return 3500.f;}
	
	//returns assigned point
	UFUNCTION(BlueprintCallable)
	virtual AActor* AssignNewPatrolPoint();
	
	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void SetTarget(AActor*_target);

	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void SetLastSeenLocation(FVector _lastSeenLocation);

	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void SetLastNoiseLocation(FVector _lastNoiseLocation);

	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void SetPatrolLocation(AActor*_PatrolNode,TArray<AActor*> _PatrolPoints);

	UFUNCTION(BlueprintPure)
	AActor* GetCurrentPatrolPoint();

	virtual EAIState GetCurrentAIState_Implementation() override;
};
