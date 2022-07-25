// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Player/RonController.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Framework/KitchenGameMode.h"

ARonController::ARonController()
{
	IsGamePaused = false;
}

void ARonController::BeginPlay()
{
	GetKitchenGamemode();
	PlayerInput->SetMouseSensitivity(.04);
}

void ARonController::GetKitchenGamemode()
{
	if (AKitchenGameMode* GM = Cast<AKitchenGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		KitchenGameMode = GM;
	}
}

void ARonController::EnableUIInput()
{
	FInputModeUIOnly InputData;
	SetInputMode(InputData);
	SetShowMouseCursor(true);
}

void ARonController::EnableGameplayInput()
{
	FInputModeGameOnly InputData;
	SetInputMode(InputData);
	SetShowMouseCursor(false);
}

void ARonController::PauseGame()
{
	
	EnableUIInput();
	IsGamePaused = true;
	UGameplayStatics::SetGamePaused(GetWorld(), IsGamePaused);
	if(PauseMenuClass)
		KitchenGameMode->ChangeMenuWidget(PauseMenuClass);
}

void ARonController::ResumeGame()
{
	EnableGameplayInput();
	IsGamePaused = false;
	UGameplayStatics::SetGamePaused(GetWorld(), IsGamePaused);

	if(PlayerHudClass)
		KitchenGameMode->ChangeMenuWidget(PlayerHudClass);
}
