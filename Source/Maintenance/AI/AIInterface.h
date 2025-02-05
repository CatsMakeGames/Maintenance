// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIState.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAINTENANCE_API IAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Called from AIController. Overriden in character
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	TArray<AActor*> GetPatrolPoints();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	EAIState GetCurrentAIState();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnSawTarget();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void OnLostTarget();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	bool CanBeSeen();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category=Sight)
	bool CanBeSeenByThisShadow(AActor*Shadow,float SightRadius);
};
