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

	UFUNCTION(BlueprintCallable)
	void OnPressResume();

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;
};
