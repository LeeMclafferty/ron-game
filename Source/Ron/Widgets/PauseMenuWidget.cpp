// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Widgets/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Player/RonController.h"
#include "Ron/Framework/KitchenGameMode.h"

bool UPauseMenuWidget::Initialize()
{
	bool success = Super::Initialize();

	if (success)
	{

		return success;
	}

	return false;
}

void UPauseMenuWidget::OnPressResume()
{
	if (ResumeButton)
	{
		if (auto RonController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			RonController->ResumeGame();
		}
	}
}

void UPauseMenuWidget::OnPressSettings()
{
	if (SettingsButton)
	{
		if (AKitchenGameMode* GM = Cast<AKitchenGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (SettingsMenuClass)
			{
				GM->ChangeMenuWidget(SettingsMenuClass);
			}
		}
	}
}
