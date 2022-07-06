// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "RonController.generated.h"

/**
 * 
 */
UCLASS()
class RON_API ARonController : public APlayerController
{
	GENERATED_BODY()

public:
	ARonController();

	void EnableUIInput();
	void EnableGameplayInput();

	UFUNCTION()
	void PauseGame();
	UFUNCTION()
	void ResumeGame();

protected:
	virtual void BeginPlay() override;

	//Widgets to switch between
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerHudClass;

private:

	void GetKitchenGamemode();
	class AKitchenGameMode* KitchenGameMode;
	bool IsGamePaused;

	
};
