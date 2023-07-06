#include "Ron/Items/LootableInventoryWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ScrollBox.h"

#include "Ron/Player/RonController.h"
#include "Ron/Player/CharacterBase.h"
#include "Ron/World/Lootable.h"
#include "Ron/Items/InventoryItemWidget.h"
#include "Ron/World/FridgeLootable.h"

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

void ULootableInventoryWidget::CloseWidget()
{
    static bool HasBeenClosed = false;
    ARonController* PlayerController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController)
    {
        PlayerController->EnableGameplayInput();
        RemoveFromParent();
        ACharacterBase* PlayerCharacter = Cast<ACharacterBase>(PlayerController->GetCharacter());
        if (PlayerCharacter)
        {
            if (AFridgeLootable* Lootable = Cast<AFridgeLootable>(PlayerCharacter->GetActorBeingLooted()))
            {
                if (SoundToPlay && !HasBeenClosed)
                {
                    HasBeenClosed = true;
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, PlayerCharacter->GetActorLocation());
                }
            }
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
        CloseButton->OnPressed.AddDynamic(this, &ULootableInventoryWidget::CloseWidget);
    }
}
