// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolBase.h"
#include "FlashlightBase.generated.h"

/**
 * 
 */
UCLASS()
class MAINTENANCE_API AFlashlightBase : public AToolBase
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	bool bActivated = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
	bool bCanBeActivated = true;
};
