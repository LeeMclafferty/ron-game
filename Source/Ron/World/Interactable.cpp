// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Interactable.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractable::Interact()
{
	
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

