// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Ingredient.h"

#include "Ron/Player/CharacterBase.h"

AIngredient::AIngredient()
{
	IsPasta = false;
	IsSalt = false;
}

void AIngredient::HandleDestruction(ACharacterBase* Character)
{
	// TODO: make disappear smoothly with particles.
	if (!Character)
		return;

	Character->SetHoldingItem(false);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("HandDestruction"))
}

void AIngredient::Interact()
{
	Super::Interact();
}
