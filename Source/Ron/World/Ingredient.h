// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/WorldPickup.h"
#include "Ingredient.generated.h"

/**
 * 
 */
UCLASS()
class RON_API AIngredient : public AWorldPickup
{
	GENERATED_BODY()

public:

	AIngredient();

	bool GetIsPasta() { return IsPasta; }
	bool GetIsSalt() { return IsSalt; }
	bool GetIsNeeded() { return IsNeeded; }

	void HandleDestruction(class ACharacterBase* Character);

protected:

	UPROPERTY(EditDefaultsOnly)
	bool IsPasta;
	UPROPERTY(EditDefaultsOnly)
	bool IsSalt;
	UPROPERTY(EditDefaultsOnly)
	bool IsNeeded;
	
	virtual void Interact() override;
};
