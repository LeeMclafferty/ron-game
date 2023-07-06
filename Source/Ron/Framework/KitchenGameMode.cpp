#include "Ron/Framework/KitchenGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Ron/World/CookingPot.h"
#include "Ron/World/Key.h"
#include "Ron/World/Door.h"
#include "Ron/Widgets/GameplayWidget.h"
#include "Ron/Framework/RonGameInstance.h"
#include "Ron/Player/RonController.h"
#include "Ron/World/StoryRadio.h"

AKitchenGameMode::AKitchenGameMode()
    : IsQuestDefault(false),
    IsOnQuestOne(false),
    IsOnQuestTwo(false),
    IsOnQuestThree(false),
    HasFoundRecipe(false),
    GI(nullptr),
    HasFinalKey(false)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AKitchenGameMode::BeginPlay()
{
    Super::BeginPlay();

    Setup();
    PC = GetController();
    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
    ChangeMenuWidget(StartingWidgetClass);
    SetupKitchenQuestsText();
    SpawnActorsOnBeginPlay();
    IsQuestDefault = true;
    StartSounds();

    if (PC)
        PC->EnableGameplayInput();

    if (FinalKey)
    {
        FinalKey->SetActorHiddenInGame(true);
        FinalKey->SetActorEnableCollision(false);
    }

    SetExitDoorKey();
}

FString AKitchenGameMode::GetKitchenQuest(int Index)
{
    if (Index < 0 || Index >= KitchenQuestsText.Num())
        return FString();

    return KitchenQuestsText[Index];
}

void AKitchenGameMode::StartSounds()
{
    if (GI)
    {
        GI->PlaySound(GI->VolumeMultiplier);
    }
}

void AKitchenGameMode::Setup()
{
    URonGameInstance* Inst = Cast<URonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!Inst)
        return;

    GI = Inst;
}

void AKitchenGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (CheckForAllIngredients())
    {
        if (HasFinalKey)
            return;

        HasFinalKey = true;
        RevealKey();
    }
    CheckQuestIndex();
    if (UGameplayWidget* GameplayWidget = Cast<UGameplayWidget>(CurrentWidget))
    {
        GameplayWidget->ChooseQuest();
    }
}

bool AKitchenGameMode::CheckForAllIngredients()
{
    if (CookingPot)
    {
        if (CookingPot->HasAllIngredients())
        {
            return true;
        }
    }
    return false;
}

void AKitchenGameMode::RevealKey()
{
    if (!FinalKey || !KeySpawnVFX)
        return;

    HasFinalKey = true;
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), KeySpawnVFX, FinalKey->GetActorLocation());
    FinalKey->SetActorHiddenInGame(false);
    FinalKey->SetActorEnableCollision(true);
}

void AKitchenGameMode::SpawnActorsOnBeginPlay()
{
    SpawnCookingPot();
    SpawnFinalKey();
    SpawnExitDoor();
}

void AKitchenGameMode::SpawnCookingPot()
{
    if (!CookingPotClass)
        return;

    FVector Scale = FVector::OneVector;
    FVector Location = FVector(-294.915436, -702.515381, 256.000519);
    FRotator Rotation = FRotator::ZeroRotator;
    FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
    FActorSpawnParameters SpawnParams;
    SpawnParams.bNoFail = true;

    CookingPot = GetWorld()->SpawnActor<ACookingPot>(CookingPotClass, SpawnTransform, SpawnParams);
}

void AKitchenGameMode::SpawnFinalKey()
{
    if (!FinalKeyClass)
        return;

    FVector Scale = FVector::OneVector;
    FVector Location = FVector(-303.353497, -695.778022, 291.443368);
    FRotator Rotation = FRotator::ZeroRotator;
    FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
    FActorSpawnParameters SpawnParams;
    SpawnParams.bNoFail = true;

    FinalKey = GetWorld()->SpawnActor<AUnlockKey>(FinalKeyClass, SpawnTransform, SpawnParams);
}

void AKitchenGameMode::SetExitDoorKey()
{
    if (!ExitDoor)
        return;

    ExitDoor->LockDoor();
    ExitDoor->SetUnlockKey(FinalKey);
}

void AKitchenGameMode::SetupKitchenQuestsText()
{
    KitchenQuestsText.Emplace("Find mom's secret recipe!");
    KitchenQuestsText.Emplace("Put all of the ingredients into the tall pot.");
    KitchenQuestsText.Emplace("Use the key to get to a safe place!");
}

void AKitchenGameMode::CheckQuestIndex()
{
    if (!HasFoundRecipe && !IsQuestDefault)
    {
        IsOnQuestOne = true;
        IsOnQuestTwo = false;
        IsOnQuestThree = false;
    }
    else if (HasFoundRecipe && !CheckForAllIngredients())
    {
        IsOnQuestOne = false;
        IsOnQuestTwo = true;
        IsOnQuestThree = false;
    }
    else if (HasFoundRecipe && CheckForAllIngredients() && HasFinalKey)
    {
        IsOnQuestOne = false;
        Here's the refactored code for the remaining part of the "KitchenGameMode" class:

            ```cpp
            IsOnQuestTwo = false;
        IsOnQuestThree = true;
    }
}

ARonController* AKitchenGameMode::GetController() const
{
    return Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AKitchenGameMode::SpawnExitDoor()
{
    if (!ExitDoorClass)
        return;

    FVector Scale = FVector::OneVector;
    FVector Location = FVector(-1544.647671, -154.974192, 465.966061);
    FRotator Rotation = FRotator(0, 90.f, 0);
    FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
    FActorSpawnParameters SpawnParams;
    SpawnParams.bNoFail = true;

    ExitDoor = GetWorld()->SpawnActor<ADoor>(ExitDoorClass, SpawnTransform, SpawnParams);
}
