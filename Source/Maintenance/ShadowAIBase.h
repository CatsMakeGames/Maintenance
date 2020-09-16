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
	
	protected:
	FTimerHandle UpdateTimerHandle;
	
	public:
	AShadowAIBase();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UAIPerceptionComponent* Senses;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<AActor*> PatrolPoints;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AMaintenanceCharacter * Target;
	
	UFUNCTION(BlueprintCallable)
	virtual void UpdatePerceivedActors();
};
