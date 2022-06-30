// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/Interactable.h"
#include "Lootable.generated.h"

/**
 * 
 */
UCLASS()
class RON_API ALootable : public AInteractable
{
	GENERATED_BODY()

public:
	ALootable();

	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items");
	TArray< class UItemData* > AllItemData;

	UItemData* ItemOne;

	void SpawnItem(TSubclassOf<class AInteractable>ItemClass);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class UUserWidget> WidgetClass;

	class ULootableInventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ActivateBox;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ItemSpawnLocation;

	void LoadInventoryWidget();
	void SetUIPlayerInput();

	
};
