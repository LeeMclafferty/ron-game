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

    FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fire"));
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
        PlaySound(StoveTick, StoveTickSound);
        GetWorld()->GetTimerManager().SetTimer(TurnOnTimerHandle, this, &AStove::TurnOn, 1.2f, false);
    }
    else
    {
        TurnOff();
    }
}

void AStove::TurnOn()
{
    PlaySound(StoveFlame, StoveFlameSound);
    BurnerLight->SetVisibility(true);
    FireMesh->SetHiddenInGame(false);
    IsTurnedOn = true;
}

void AStove::TurnOff()
{
    StoveTickSound->VolumeMultiplier = 0.f;
    StoveFlameSound->VolumeMultiplier = 0.f;
    BurnerLight->SetVisibility(false);
    FireMesh->SetHiddenInGame(true);
    IsTurnedOn = false;
}

void AStove::PlaySound(USoundBase* SoundToPlay, USoundCue* SoundCue)
{
    if (!SoundToPlay || !SoundCue)
        return;

    SoundCue->VolumeMultiplier = 1.f;
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, BurnerBox->GetComponentLocation());
}
