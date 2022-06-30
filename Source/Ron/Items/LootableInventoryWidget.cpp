// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Items/LootableInventoryWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"

#include "Ron/Player/RonController.h"
#include "Ron/Player/CharacterBase.h"
#include "Ron/World/Lootable.h"
#include "Ron/Items/InventoryItemWidget.h"

bool ULootableInventoryWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (Success)
	{
		InitWidget();
		SetupScrollBox();
	}

	return Success;
}

void ULootableInventoryWidget::CloseWiget()
{
	if (ARonController* PlayerContoller = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerContoller->EnableGameplayInput();
		RemoveFromParent();
		if (ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(PlayerContoller->GetCharacter()))
		{
			PlayerCharacter->SetActorBeingLooted(nullptr);
		}
	}
}

void ULootableInventoryWidget::InitWidget()
{
	SetupCloseButton();
}

void ULootableInventoryWidget::SetupCloseButton()
{
	if (CloseButton)
	{
		CloseButton->OnPressed.AddDynamic(this, &ULootableInventoryWidget::CloseWiget);
	}
}


