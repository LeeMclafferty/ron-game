#include "Ron/World/Interactable.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractable::Interact()
{
	// Empty implementation. Can be overridden in derived classes.
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
