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

	UFUNCTION(BlueprintImplementableEvent)
	void SetupScrollBox();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearScrollBox();

	UPROPERTY(BlueprintReadWrite)
	TArray<class UInventoryItemWidget*> ItemWidgets;


protected:


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> InventoryItemWidgetClass;
	
	void InitWidget();
	void SetupCloseButton();

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* SoundToPlay;
	
};
