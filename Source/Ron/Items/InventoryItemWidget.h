// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"


UCLASS()
class RON_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;
	UPROPERTY(EditAnywhere, Instanced)
	class ALootable* ActorBeingLooted;

	UFUNCTION(BlueprintCallable)
	void InitItem(ALootable* Lootable, int Index);

protected:
	UFUNCTION(BlueprintCallable)
	void SetupThumbnail(class ALootable* Lootable, int Index);
	UFUNCTION(BlueprintCallable)
	void SetupDisplayName(class ALootable* Lootable, int Index);
	UFUNCTION(BlueprintCallable)
	void SetupDescription(class ALootable* Lootable, int Index);
	UFUNCTION()
	void SetupSpawnButton();

	UPROPERTY(BlueprintReadWrite)
	int32 ButtonIndex;
private:

	UPROPERTY(meta = (BindWidget))
	class UImage* ThumbnailImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;

	//Still need to set up.
	UPROPERTY(meta = (BindWidget))
	class UButton* SpawnButton;

	UFUNCTION()
	void SpawnItemFromLootable();

	UFUNCTION()
	void DisableClick();

	UFUNCTION()
	void EnableClick();

	
};
