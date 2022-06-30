// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Sink.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


ASink::ASink()
{
	WaterPath = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Water Path"));
	WaterPath->SetupAttachment(RootComponent);

	HandleTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Handle Trigger"));
	HandleTrigger->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	WaterMesh->SetupAttachment(RootComponent);
	WaterMesh->SetHiddenInGame(true);
	WaterMesh->SetCollisionProfileName("NoCollision");
}

void ASink::Interact()
{
	// TODO: Add Water VFX and sound.
	if (!TurnedOn)
	{
		TurnedOn = true;
		WaterMesh->SetHiddenInGame(false);
	}
	else
	{
		TurnedOn = false;
		WaterMesh->SetHiddenInGame(true);
	}
}
