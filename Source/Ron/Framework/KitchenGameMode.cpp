// Fill out your copyright notice in the Description page of Project Settings.


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
:IsQuestDefault(false), 
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
	SetupKitchQuestsText();
	SpawnActorsOnBeginPlay();
	IsQuestDefault = true;
	StartSounds();


	if(PC)
		PC->EnableGameplayInput();

	if (FinalKey)
	{
		FinalKey->SetActorHiddenInGame(true);
		FinalKey->SetActorEnableCollision(false);
	}

	SetExitDoorKey();
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
		if(CookingPot->HasAllIngrediants())
		{
			return true;
		}
	}
	return false;
}

void AKitchenGameMode::RevealKey()
{
	if (!FinalKey || !KeySpawnVFX )
		return;

	HasFinalKey = true;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), KeySpawnVFX, FinalKey->GetActorLocation());
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), (USoundBase*)KeySpawnSound);
	FinalKey->SetActorHiddenInGame(false);
	FinalKey->SetActorEnableCollision(true);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("RevealKey")));
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

void AKitchenGameMode::SetupKitchQuestsText()
{
	FString QuestDefault("");
	FString QuestOne("Find mom's secret recipe!");
	FString QuestTwo("Put all of the ingredients into the tall pot.");
	FString QuestThree("Use the key to get to a safe place!");

	KitchenQuestsText.Emplace(QuestOne);
	KitchenQuestsText.Emplace(QuestTwo);
	KitchenQuestsText.Emplace(QuestThree);
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
	FVector Scale = FVector::OneVector;
	FVector Location = FVector(-1544.647671, -154.974192, 465.966061);
	FRotator Rotation = FRotator(0, 90.f, 0);
	FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
	FActorSpawnParameters SpawnParams;

	SpawnParams.bNoFail = true;

	if (ExitDoorClass)
	{
		ExitDoor = GetWorld()->SpawnActor<ADoor>(ExitDoorClass, SpawnTransform, SpawnParams);
	}
}

