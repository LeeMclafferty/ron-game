// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Widgets/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Player/RonController.h"

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
