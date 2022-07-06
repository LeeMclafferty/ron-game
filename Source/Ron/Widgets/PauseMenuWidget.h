// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RON_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	bool Initialize() override;

protected:
	UFUNCTION(BlueprintCallable)
	void OnPressResume();

	UFUNCTION(BlueprintCallable)
	void OnPressSettings();

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsButton;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SettingsMenuClass;
};
