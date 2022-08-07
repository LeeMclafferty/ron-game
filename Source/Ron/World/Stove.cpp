// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/Stove.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AStove::AStove()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	BurnerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Burner Box"));
	BurnerBox->SetupAttachment(RootComponent);

	ButtonBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonBox"));
	ButtonBox->SetupAttachment(RootComponent);

	BurnerLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Burner Light"));
	BurnerLight->SetupAttachment(StaticMesh);
	BurnerLight->SetVisibility(false);

	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>("Fire");
	FireMesh->SetupAttachment(RootComponent);
	FireMesh->SetHiddenInGame(true);

	IsTurnedOn = false;
	
}

void AStove::Interact()
{
	// TODO: turned on when interacting with the buttons
	// TODO: have a red light when turned on
	// TODO: If Stove is on and CookingPot is on burning, then CookingPotHasFire
	// TODO: Add Fire VFX
	if (!IsTurnedOn)
	{
		PlaySound((USoundBase*)StoveTick, StoveTick);
		FTimerHandle OnTimer;
		GetWorld()->GetTimerManager().SetTimer(OnTimer, this, &AStove::TurnOn, 1.2, false);
	}
	else
	{
		TurnOff();
	}
}

void AStove::TurnOn()
{
	PlaySound((USoundBase*)StoveFlame, StoveFlame);
	BurnerLight->SetVisibility(true);
	FireMesh->SetHiddenInGame(false);
	IsTurnedOn = true;
}

void AStove::TurnOff()
{
	StoveTick->VolumeMultiplier = 0;
	StoveFlame->VolumeMultiplier = 0;
	BurnerLight->SetVisibility(false);
	FireMesh->SetHiddenInGame(true);
	IsTurnedOn = false;
}

void AStove::PlaySound(USoundBase* SoundtoPlay, USoundCue* SoundCue)
{
	if (!SoundtoPlay || !SoundCue)
		return;

	SoundCue->VolumeMultiplier = 1;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundtoPlay, BurnerBox->GetComponentLocation());
}

