// Copyright Epic Games, Inc. All Rights Reserved.

#include "RonGameMode.h"
#include "RonHUD.h"
#include "RonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Framework/RonGameInstance.h"

ARonGameMode::ARonGameMode()
	: Super(),
	CurrentWidget(nullptr)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ARonHUD::StaticClass();
}

void ARonGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (auto* GameInst = Cast<URonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInst->LoadGame();
	}
}

void ARonGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
