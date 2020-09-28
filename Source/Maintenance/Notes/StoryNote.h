// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Engine/UserDefinedStruct.h"
#include "StoryNote.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStoryNote: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString DevName = "Story";

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString DisplayName = "StoryNote";

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText Text;
};
