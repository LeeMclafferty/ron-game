// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Key.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Player/CharacterBase.h"

AUnlockKey::AUnlockKey()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	Trigger->SetupAttachment(RootComponent);

	StaticMesh->SetSimulatePhysics(false);
}

void AUnlockKey::BeginPlay()
{
	Super::BeginPlay();

	SetupRefs();
}

void AUnlockKey::SetupRefs()
{
	if(auto PlayerChar = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		Character = PlayerChar;
}

void AUnlockKey::DisableKey()
{	
	//Add FX here later
	if (!Character)
		return;

	Character->SetHoldingItem(false);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("HandDestruction"))
}

void AUnlockKey::Interact()
{
	Super::Interact();

}
