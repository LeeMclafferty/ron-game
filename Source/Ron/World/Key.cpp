// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Key.h"
#include "Components/BoxComponent.h"

AUnlockKey::AUnlockKey()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	Trigger->SetupAttachment(RootComponent);

	StaticMesh->SetSimulatePhysics(false);
}

void AUnlockKey::DisableKey()
{	
	//Add FX here later.
	Destroy();
}

void AUnlockKey::Interact()
{
	Super::Interact();

}
