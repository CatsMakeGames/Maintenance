// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task.h"
#include "TaskInterface.h"
#include "Components/ActorComponent.h"
#include "TaskManagmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskFromListCompletedDelegate,FTask, task);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewTaskAddedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskFromListProgressedDelegate,FTask,task,int,amount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINTENANCE_API UTaskManagmentComponent : public UActorComponent, public ITaskInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTaskManagmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnTaskFromListCompletedDelegate OnTaskFromListCompleted;
	
	UPROPERTY(BlueprintAssignable)
	FOnTaskFromListProgressedDelegate OnTaskFromListProgressed;
	
	UPROPERTY(BlueprintAssignable)
	FOnNewTaskAddedDelegate OnNewTaskAdded;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	TArray<FTask>CurrentTasks;

	UFUNCTION(BlueprintCallable)
	virtual void AddTask(FTask task);

	virtual void OnTaskCompleted_Implementation(FTask task) override;

	virtual void OnTaskProgressed_Implementation(FTask task, int amount) override;
};
