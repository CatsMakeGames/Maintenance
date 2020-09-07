// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MaintenanceHUD.generated.h"

UCLASS()
class AMaintenanceHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMaintenanceHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

