// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskManagmentComponent.h"

// Sets default values for this component's properties
UTaskManagmentComponent::UTaskManagmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTaskManagmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTaskManagmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTaskManagmentComponent::OnTaskCompleted_Implementation(FTask task)
{
	//when task is completed event is called and task is removed from list
	if (CurrentTasks.Num() > 0)
	{
		for (int i = 0; i < CurrentTasks.Num(); i++)
		{
			if(CurrentTasks[i].Name == task.Name)
			{
				OnTaskFromListCompleted.Broadcast(task);
				CurrentTasks.RemoveAt(i);
				return;
			}
		}
	}
}

void UTaskManagmentComponent::OnTaskProgressed_Implementation(FTask task, int amount)
{
	//when task is completed event is called and task is removed from list
	if (CurrentTasks.Num() > 0)
	{
		for (int i = 0; i < CurrentTasks.Num(); i++)
		{
			if (CurrentTasks[i].Name == task.Name)
			{
				if (CurrentTasks[i].CurrentCount + amount >= CurrentTasks[i].NeededCount)
				{
					OnTaskFromListCompleted.Broadcast(task);
					CurrentTasks.RemoveAt(i);
				}
				else
				{
					CurrentTasks[i].CurrentCount += amount;
					OnTaskFromListProgressed.Broadcast(task, amount);
				}

				return;
			}
		}
	}
}

