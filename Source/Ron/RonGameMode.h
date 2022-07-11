// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RonGameMode.generated.h"

UCLASS(minimalapi)
class ARonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARonGameMode();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY()
	class UUserWidget* CurrentWidget;
protected:
	virtual void BeginPlay() override;

};



