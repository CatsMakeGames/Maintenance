// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "Task.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTask: public FTableRowBase
{
 GENERATED_BODY()
public:
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 FString Name;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 FText DisplayName;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 int NeededCount = 1;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere)
 int CurrentCount = 0;
};
