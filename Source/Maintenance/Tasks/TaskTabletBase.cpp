// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskTabletBase.h"

#include "TaskManagmentComponent.h"

// Sets default values
ATaskTabletBase::ATaskTabletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TabletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletMesh"));
	TabletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = TabletMesh;
}

// Called when the game starts or when spawned
void ATaskTabletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATaskTabletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATaskTabletBase::Update()
{
	if (TabletMesh != nullptr)
	{
		if (TaskDisplayTexts.Num() > 0)
		{
			for (int i = 0; i < TaskDisplayTexts.Num(); i++)
			{
				TaskDisplayTexts[i]->DestroyComponent(); //clear previous text
			}
		}


		if (TaskManager == nullptr)
		{
			if (GetOwner() != nullptr)
			{
				if (GetOwner()->FindComponentByClass(UTaskManagmentComponent::StaticClass()) != nullptr)
				{
					TaskManager = Cast<UTaskManagmentComponent>(GetOwner()->FindComponentByClass(UTaskManagmentComponent::StaticClass()));
				}
			}
			else
			{
				return;
			}
		}

		if (TaskManager != nullptr)
		{
			if (TaskManager->CurrentTasks.Num() > 0)
			{
				for (int i = 0; i < TaskManager->CurrentTasks.Num(); i++)
				{
					UTextRenderComponent* text = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
					if (text != nullptr)
					{
						text->RegisterComponent();
						text->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						text->AddRelativeLocation(FVector(i * 10,15,0));
						text->SetRelativeRotation(FRotator(90,0,-180));
						text->SetWorldSize(5);
					}

					text->SetText
					(
					FText::FromString
					(
						TaskManager->CurrentTasks[i].DisplayName.ToString() + "[" +
						FString::FromInt(TaskManager->CurrentTasks[i].CurrentCount) + "/" +
						FString::FromInt(TaskManager->CurrentTasks[i].NeededCount) + "]"
					)
					);
					TaskDisplayTexts.Add(text);
					
				}
			}
		}
	}
}


