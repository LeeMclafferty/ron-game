// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Ingredient.h"

AIngredient::AIngredient()
{
	IsPasta = false;
	IsSalt = false;
}

void AIngredient::HandleDestruction()
{
	// TODO: make disappear smoothly with particles.

	Destroy();
}

void AIngredient::Interact()
{
	Super::Interact();
}
