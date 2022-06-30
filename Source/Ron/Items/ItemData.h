// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class RON_API UItemData : public UObject
{
	GENERATED_BODY()

public:
	UItemData();

	UTexture2D* GetTexture() { return Thumbnail; }
	FText GetDisplayName() { return DisplayName; }
	FText GetDescription() { return Description; }
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AInteractable> ItemClass;

private:
	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* Thumbnail;

	
};
