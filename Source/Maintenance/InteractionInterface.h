// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Door/KeyComponent.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAINTENANCE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Interact(AActor*Interactor,UActorComponent*InteractedComponent);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void UseItem(AActor*Item,UActorComponent*InteractedComponent);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void UseItemInHand(AActor*HoldingActor);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Keys)
	void UseKeys(AActor * interactor,const  TArray<FKeyInfo>& Keys);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category=Keys)
    void RemoveKey(const FString& keyName);
};
