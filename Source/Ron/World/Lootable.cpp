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
}

void ALootable::Interact()
{
	Super::Interact();
	LoadInventoryWidget();
}

void ALootable::Tick(float DeltaSeconds)
{

}

void ALootable::LoadInventoryWidget()
{
	if (ARonController* PlayerController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (WidgetClass)
		{
			if (ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(PlayerController->GetCharacter()))
			{
				PlayerCharacter->SetActorBeingLooted(this);
			}

			InventoryWidget = CreateWidget<ULootableInventoryWidget>(PlayerController, WidgetClass);
			InventoryWidget->AddToViewport();
			SetUIPlayerInput();
		}
	}
}

void ALootable::SetUIPlayerInput()
{
	if (ARonController* PlayerController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->EnableUIInput();
	}
}

void ALootable::RemoveByIndex(int32 Index)
{
	AllItemData.RemoveAt(Index);
}

void ALootable::SpawnItem(TSubclassOf<class AInteractable> ItemClass)
{
	if (ItemClass)
	{
		FVector Scale = FVector::OneVector;
		FVector Location = ItemSpawnLocation->GetComponentLocation();
		FRotator Rotation = GetActorRotation();
		FTransform SpawnTransform(Rotation, Location, Scale);
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;

		GetWorld()->SpawnActor<AInteractable>(ItemClass, SpawnTransform, SpawnParams);
	}
}
