// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maintenance/InteractionInterface.h"
#include "Maintenance/Pickup/HoldableActorComponent.h"

#include "ToolBase.generated.h"

//tools are specific actors that player can carry around and use(like axe or crowbar)
UCLASS()
class MAINTENANCE_API AToolBase : public AActor,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToolBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UHoldableActorComponent*Holdable;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
