// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "AIState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
 EAS_Normal  UMETA(DisplayName = "Normal"),
 EAS_Chase  UMETA(DisplayName = "Chase")
};