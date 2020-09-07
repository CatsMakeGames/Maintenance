// Copyright Epic Games, Inc. All Rights Reserved.

#include "MaintenanceGameMode.h"
#include "MaintenanceHUD.h"
#include "MaintenanceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMaintenanceGameMode::AMaintenanceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMaintenanceHUD::StaticClass();
}
