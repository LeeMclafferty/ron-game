// Fill out your copyright notice in the Description page of Project Settings.

#include "Ron/World/WorldPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

#include "Ron/Player/CharacterBase.h"


AWorldPickup::AWorldPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionProfileName("BlockAll");
	// If Mass is too low, object seem to fly off into no where. 
	StaticMesh->SetMassOverrideInKg(NAME_None, 100.f, true);
	StaticMesh->SetSimulatePhysics(true);
	
	IsBeingHeld = false;
}

void AWorldPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldPickup::Interact()
{
	Super::Interact();

	if (ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (!IsBeingHeld)
		{
			Player->SetHoldingItem(true);
			Player->SetHeldActor(this);
			IsBeingHeld = true;
			StaticMesh->SetSimulatePhysics(false);
			StaticMesh->SetCollisionProfileName("IgnoreOnlyPawn");
		}
		else
		{
			Player->SetHoldingItem(false);
			Player->SetHeldActor(nullptr);
			IsBeingHeld = false;
			StaticMesh->SetSimulatePhysics(true);
			StaticMesh->SetCollisionProfileName("BlockAll");
		}
	}
}

void AWorldPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

