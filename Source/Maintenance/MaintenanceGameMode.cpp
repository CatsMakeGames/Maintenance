// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaintenanceGameMode.h"
#include "MaintenanceHUD.h"
#include "MaintenanceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMaintenanceGameMode::AMaintenanceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	
	DefaultPawnClass = AMaintenanceCharacter::StaticClass();

	// use our custom HUD class
	HUDClass = AMaintenanceHUD::StaticClass();
}
