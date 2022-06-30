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
	KitchenGameMode->ChangeMenuWidget(PauseMenuClass);
}

void ARonController::ResumeGame()
{
	EnableGameplayInput();
	IsGamePaused = false;
	UGameplayStatics::SetGamePaused(GetWorld(), IsGamePaused);
	KitchenGameMode->ChangeMenuWidget(PlayerHud);
}