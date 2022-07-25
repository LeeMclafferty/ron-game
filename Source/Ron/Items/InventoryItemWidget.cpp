// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Items/InventoryItemWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Ron/Player/CharacterBase.h"
#include "Ron/World/Lootable.h"
#include "Ron/Items/ItemData.h"
#include "Ron/Items/LootableInventoryWidget.h"

bool UInventoryItemWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (Success)
	{
		
		
	}

	return Success;
}

void UInventoryItemWidget::InitItem(ALootable* Lootable, int Index)
{
	ActorBeingLooted = Lootable;
	ButtonIndex = Index;
	Initialize();
	SetupThumbnail(Lootable, Index);
	SetupDisplayName(Lootable, Index);
	SetupDescription(Lootable, Index);
	SetupSpawnButton();
	//UE_LOG(LogTemp, Warning, TEXT("Item init"));
}

void UInventoryItemWidget::SetupThumbnail(class ALootable* Lootable, int Index)
{
	if (ThumbnailImage)
	{
		ThumbnailImage->SetBrushFromTexture(Lootable->AllItemData[Index]->GetTexture());
	}
}

void UInventoryItemWidget::SetupDisplayName(class ALootable* Lootable, int Index)
{
	if (DisplayName)
	{
		DisplayName->SetText(Lootable->AllItemData[Index]->GetDisplayName());
	}
}

void UInventoryItemWidget::SetupDescription(class ALootable* Lootable, int Index)
{
	if (DescriptionText)
	{
		DescriptionText->SetText(Lootable->AllItemData[Index]->GetDescription());
	}
}

void UInventoryItemWidget::SetupSpawnButton()
{
	if (SpawnButton)
	{
		SpawnButton->OnPressed.AddDynamic(this, &UInventoryItemWidget::SpawnItemFromLootable);
	}
}

void UInventoryItemWidget::SpawnItemFromLootable()
{
	if (ActorBeingLooted)
	{
		ActorBeingLooted->SpawnItem(ActorBeingLooted->AllItemData[ButtonIndex]->ItemClass);
		ActorBeingLooted->RemoveByIndex(ButtonIndex);
		RemoveFromParent();
		ActorBeingLooted->GetInventoryWidget()->SetupScrollBox();
	}
	
}

void UInventoryItemWidget::DisableClick()
{
	if (auto* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->bEnableClickEvents = false;
	}
}

void UInventoryItemWidget::EnableClick()
{
	if (auto* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->bEnableClickEvents = true;
	}
}
