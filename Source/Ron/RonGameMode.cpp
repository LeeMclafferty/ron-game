// Copyright Epic Games, Inc. All Rights Reserved.

#include "RonGameMode.h"
#include "RonHUD.h"
#include "RonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARonGameMode::ARonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARonHUD::StaticClass();
}
