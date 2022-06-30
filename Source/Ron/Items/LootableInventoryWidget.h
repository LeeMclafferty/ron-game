// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootableInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RON_API ULootableInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void CloseWiget();
	bool Initialize() override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void SetupScrollBox();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> InventoryItemWidgetClass;
	
	void InitWidget();
	void SetupCloseButton();
	
};
