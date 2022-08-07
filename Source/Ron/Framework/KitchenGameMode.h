// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/RonGameMode.h"
#include "KitchenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RON_API AKitchenGameMode : public ARonGameMode
{
	GENERATED_BODY()

public:
	AKitchenGameMode();

	void ObtainedFinalKey() { HasFinalKey = true; }
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintPure)
	bool CanActivateLights() { return HasFinalKey ? true :false; }

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	FString GetKitchenQuest(int index);
	
	bool IsQuestDefault;
	bool IsOnQuestOne;
	bool IsOnQuestTwo;
	bool IsOnQuestThree;

	bool HasFoundRecipe;

	UFUNCTION(BlueprintCallable)
	bool DoesHaveFinalKey() { return HasFinalKey; }
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> StartingWidgetClass;

	void StartSounds();

private:
	class URonGameInstance* GI;
	void Setup();
	bool HasFinalKey;

	class ACookingPot* CookingPot;
	class AUnlockKey* FinalKey;
	class AStoryRadio* FinalRadio;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACookingPot> CookingPotClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AUnlockKey> FinalKeyClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADoor> ExitDoorClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AStoryRadio> FinalRadioClass;

	bool CheckForAllIngredients();
	void RevealKey();
	void SpawnActorsOnBeginPlay();
	void SpawnCookingPot();
	void SpawnFinalKey();
	void SpawnExitDoor();

	class ADoor* ExitDoor;

	void SetExitDoorKey();

	TArray<FString> KitchenQuestsText;
	void SetupKitchQuestsText();

	void CheckQuestIndex();

	class ARonController* GetController() const;
	class ARonController* PC;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* KeySpawnVFX;


};
