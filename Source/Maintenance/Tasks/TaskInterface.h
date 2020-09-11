// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "UObject/Interface.h"
#include "TaskInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTaskInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAINTENANCE_API ITaskInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Completes task
	//use this only if you need to force completion of the task(f.e. skipping task)
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnTaskCompleted(FTask task);

	//Add progress to the task completion
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void OnTaskProgressed(FTask task, int amount);
};
