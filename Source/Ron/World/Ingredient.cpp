#include "Ron/World/Ingredient.h"
#include "Ron/Player/CharacterBase.h"

AIngredient::AIngredient()
{
	IsPasta = false;
	IsSalt = false;
}

void AIngredient::HandleDestruction(ACharacterBase* Character)
{
	if (!Character)
		return;

	Character->SetHoldingItem(false);
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("HandleDestruction"));
}

void AIngredient::Interact()
{
	Super::Interact();
}
