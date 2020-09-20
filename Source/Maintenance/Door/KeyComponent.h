// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyComponent.generated.h"

USTRUCT(BlueprintType)
struct FKeyInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int KeyId = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString Name = "Key";

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool OneTimeUse = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINTENANCE_API UKeyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	FKeyInfo KeyInfo;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	bool bCanBePickedUp = true;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	bool bKeyWasPickedUp = false;
};
