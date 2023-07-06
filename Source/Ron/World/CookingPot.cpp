#include "Ron/World/CookingPot.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/World/Ingredient.h"
#include "Ron/World/Sink.h"
#include "Ron/World/Stove.h"
#include "Ron/Player/CharacterBase.h"

ACookingPot::ACookingPot()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	IngredientSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("IngredientSpawn"));
}

void ACookingPot::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->SetMassOverrideInKg(NAME_None, 2000.f);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACookingPot::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACookingPot::OnOverlapEnd);

	SetupRefs();
}

void ACookingPot::Interact()
{
	Super::Interact();
}

void ACookingPot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PlaySound();

	if (HasAnyIngredients() && IsTippedOver())
	{
		OnTipOver();
	}

	UpdateOverlaps(true);
}

void ACookingPot::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AIngredient* Ingredient = Cast<AIngredient>(OtherActor))
	{
		if (Ingredient->bIsNeeded)
		{
			if (Ingredient->bIsPasta)
			{
				HasPasta = true;
				Ingredient->HandleDestruction(Character);
			}
			else if (Ingredient->bIsSalt)
			{
				HasSalt = true;
				Ingredient->HandleDestruction(Character);
			}
		}
	}
	else if (ASink* Sink = Cast<ASink>(OtherActor))
	{
		if (Sink->bIsOn)
		{
			HasWater = true;
		}
	}
	else if (AStove* Stove = Cast<AStove>(OtherActor))
	{
		if (Stove->bIsOn)
		{
			HasFire = true;
		}
	}
}

void ACookingPot::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AStove* Stove = Cast<AStove>(OtherActor))
	{
		HasFire = false;
	}
}

bool ACookingPot::HasAnyIngredients() const
{
	return (HasWater || HasSalt || HasPasta);
}

bool ACookingPot::IsTippedOver() const
{
	const float MaxTilt = 80.f;
	const float Roll = GetActorRotation().Roll;
	const float Pitch = GetActorRotation().Pitch;

	return (FMath::Abs(Roll) > MaxTilt || FMath::Abs(Pitch) > MaxTilt);
}

void ACookingPot::OnTipOver()
{
	if (HasAnyIngredients())
	{
		FVector Scale = FVector::OneVector;
		FVector Location = IngredientSpawn->GetComponentLocation();
		FRotator Rotation = GetActorRotation();
		FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;

		if (HasPasta && PastaClass)
		{
			Pasta = GetWorld()->SpawnActor<AIngredient>(PastaClass, SpawnTransform, SpawnParams);
			HasPasta = false;
		}

		if (HasSalt && SaltClass)
		{
			Salt = GetWorld()->SpawnActor<AIngredient>(SaltClass, SpawnTransform, SpawnParams);
			HasSalt = false;
		}
	}
}

void ACookingPot::PlaySound()
{
	if (HasWater && HasSalt && HasPasta && !HasPlayedSound && FireSound)
	{
		HasPlayedSound = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void ACookingPot::SetupRefs()
{
	Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool ACookingPot::HasAllIngredients() const
{
	return (HasWater && HasSalt && HasPasta && HasFire);
}
