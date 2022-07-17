// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Lootable.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "Ron/Player/RonController.h"
#include "Ron/Player/CharacterBase.h"
#include "Ron/Items/ItemData.h"
#include "Ron/Items/LootableInventoryWidget.h"

ALootable::ALootable()
{
	ActivateBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation Box"));
	RootComponent = ActivateBox;

	ItemSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Item Spawn Location"));
}

void ALootable::BeginPlay()
{
	Super::BeginPlay();

	if(AllItemData[0])
		ItemOne = AllItemData[0];
}

void ALootable::Interact()
{
	// When the player interacts with the Lootable, UI should display with what is inside.
	Super::Interact();

	LoadInventoryWidget();
}

void ALootable::Tick(float DeltaSeconds)
{
	//GEngine->AddOnScreenDebugMessage(1, 1, FColor::Purple, FString::Printf(TEXT("ItemData[0]: %ul"), AllItemData[0]));
}

void ALootable::LoadInventoryWidget()
{
	if (ARonController* PlayerContoller = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (WidgetClass)
		{
			if (ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(PlayerContoller->GetCharacter()))
			{
				PlayerCharacter->SetActorBeingLooted(this);
			}
			InventoryWidget = CreateWidget<ULootableInventoryWidget>(PlayerContoller, WidgetClass);
			InventoryWidget->AddToViewport();
			SetUIPlayerInput();
		}
	}
}

void ALootable::SetUIPlayerInput()
{
	if (ARonController* PlayerContoller = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerContoller->EnableUIInput();
	}
}

void ALootable::RemoveByIndex(int32 index)
{
	AllItemData.RemoveAt(index);
}

void ALootable::SpawnItem(TSubclassOf<class AInteractable>ItemClass)
{
	FVector Scale = FVector::OneVector;
	FVector Location = ItemSpawnLocation->GetComponentLocation();
	FRotator Rotation = GetActorRotation();
	FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;

	if (ItemClass)
	{
		GetWorld()->SpawnActor<AInteractable>(ItemClass, SpawnTransform, SpawnParams);
	}
}

