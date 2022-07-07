// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/Framework/KitchenGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


#include "Ron/World/CookingPot.h"
#include "Ron/World/Key.h"
#include "Ron/World/Door.h"
#include "Ron/Widgets/GameplayWidget.h"
#include "Ron/Framework/RonGameInstance.h"

AKitchenGameMode::AKitchenGameMode()
:IsOnQuestOne(false), IsOnQuestTwo(false), IsOnQuestThree(false), HasFoundRecipe(false), CurrentWidget(nullptr), GI(nullptr), HasFinalKey(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AKitchenGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetGamInstance();
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
	ChangeMenuWidget(StartingWidgetClass);
	SetupKitchQuestsText();
	SpawnActorsOnBeginPlay();
	IsOnQuestOne = true;
	StartSounds();
	

	if (FinalKey)
	{
		FinalKey->SetActorHiddenInGame(true);
		FinalKey->SetActorEnableCollision(false);
	}

	SetExitDoorKey();
}

void AKitchenGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

FString AKitchenGameMode::GetKitchenQuest(int index)
{
	return KitchenQuestsText[index];
}

void AKitchenGameMode::StartSounds()
{
	if (GI)
	{
		//UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GI->GetMasterSoundMix(), GI->GetMasterSoundClass(), 1.f);
		GI->PlaySound();
	}
}

void AKitchenGameMode::SetGamInstance()
{
	if (URonGameInstance* Inst = Cast<URonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI = Inst;
	}
}

void AKitchenGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CheckForAllIngredients())
	{
		if (!HasFinalKey)
		{
			HasFinalKey = true;
			RevealKey();
		}
	}
	CheckQuestIndex();
	if (UGameplayWidget* GameplayWidget = Cast<UGameplayWidget>(CurrentWidget))
	{
		GameplayWidget->ChooseQuest();
		//GEngine->AddOnScreenDebuGIessage(-1, 2, FColor::Purple, FString::Printf(TEXT("HasRecipe: %i"), CheckForAllIngredients()));
	}
}

bool AKitchenGameMode::CheckForAllIngredients()
{
	if (CookingPot)
	{
		if(CookingPot->HasAllIngrediants())
		{
			return true;
		}
	}
	return false;
}

void AKitchenGameMode::RevealKey()
{
	if (FinalKey)
	{
		HasFinalKey = true;
		FinalKey->SetActorHiddenInGame(false);
		FinalKey->SetActorEnableCollision(true);
	}
}

void AKitchenGameMode::SpawnActorsOnBeginPlay()
{
	SpawnCookingPot();
	SpawnFinalKey();
	SpawnExitDoor();
}

void AKitchenGameMode::SpawnCookingPot()
{
	FVector Scale = FVector::OneVector;
	FVector Location = FVector(-294.915436, -702.515381, 256.000519);
	FRotator Rotation = FRotator::ZeroRotator;
	FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
	FActorSpawnParameters SpawnParams;

	SpawnParams.bNoFail = true;

	if (CookingPotClass)
	{
		CookingPot = GetWorld()->SpawnActor<ACookingPot>(CookingPotClass, SpawnTransform, SpawnParams);
	}
}

void AKitchenGameMode::SpawnFinalKey()
{
	FVector Scale = FVector::OneVector;
	FVector Location = FVector(-448.107635, -686.612488, 296.153015);
	FRotator Rotation = FRotator::ZeroRotator;
	FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
	FActorSpawnParameters SpawnParams;

	SpawnParams.bNoFail = true;

	if (FinalKeyClass)
	{
		FinalKey = GetWorld()->SpawnActor<AUnlockKey>(FinalKeyClass, SpawnTransform, SpawnParams);
	}
}

void AKitchenGameMode::SetExitDoorKey()
{
	if (ExitDoor)
	{
		ExitDoor->LockDoor();
		ExitDoor->SetUnlockKey(FinalKey);
	}
}

void AKitchenGameMode::SetupKitchQuestsText()
{
	FString QuestOne("Find mom's secret recipe!");
	FString QuestTwo("Put all of the ingredients into the tall pot.");
	FString QuestThree("Use the key to get to a safe place!");

	KitchenQuestsText.Emplace(QuestOne);
	KitchenQuestsText.Emplace(QuestTwo);
	KitchenQuestsText.Emplace(QuestThree);
}

void AKitchenGameMode::CheckQuestIndex()
{
	if (!HasFoundRecipe)
	{
		IsOnQuestOne = true;
		IsOnQuestTwo = false;
		IsOnQuestThree = false;
	}
	if (HasFoundRecipe && !CheckForAllIngredients())
	{
		IsOnQuestOne = false;
		IsOnQuestTwo = true;
		IsOnQuestThree = false;
	}
	if (HasFoundRecipe && CheckForAllIngredients() && HasFinalKey)
	{
		IsOnQuestOne = false;
		IsOnQuestTwo = false;
		IsOnQuestThree = true;
	}
}

void AKitchenGameMode::SpawnExitDoor()
{
	FVector Scale = FVector(0.500000, 0.500000, 0.541599);
	FVector Location = FVector( -1556.413940, -157.914383, 436.647308);
	FRotator Rotation = FRotator(0, 90.f, 0);
	FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
	FActorSpawnParameters SpawnParams;

	SpawnParams.bNoFail = true;

	if (ExitDoorClass)
	{
		ExitDoor = GetWorld()->SpawnActor<ADoor>(ExitDoorClass, SpawnTransform, SpawnParams);
	}
}