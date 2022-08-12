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

	Character->SetHeldActor(nullptr);
	Destroy();
}

void AIngredient::Interact()
{
	Super::Interact();
}
