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
 UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
 FString Name;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
 FText DisplayName;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
 int NeededCount = 1;
 
 UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
 int CurrentCount = 0;

 UPROPERTY(BlueprintReadWrite,EditAnywhere,SaveGame)
 bool bCompleted = false;
};
