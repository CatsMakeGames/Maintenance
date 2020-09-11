// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskManagmentComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "TaskTabletBase.generated.h"

UCLASS()
class MAINTENANCE_API ATaskTabletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskTabletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Mesh)
	UStaticMeshComponent*TabletMesh;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category=Text)
	TArray<UTextRenderComponent*> TaskDisplayTexts;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UTaskManagmentComponent*TaskManager;

	//Deletes all text and spawns new. CALL ONLY ON CHANGE OF TASKS TO IMPROVE PERFORMANCE
	UFUNCTION(BlueprintCallable)
	virtual void Update();
};
