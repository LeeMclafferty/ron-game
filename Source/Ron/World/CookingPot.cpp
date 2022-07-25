

#include "Ron/World/CookingPot.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


#include "Ron/World/Ingredient.h"
#include "Ron/World/Sink.h"
#include "Ron/World/Stove.h"

ACookingPot::ACookingPot()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	Trigger->SetupAttachment(RootComponent);

	IngredientSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Ingredient Spawn"));
	IngredientSpawn->SetupAttachment(RootComponent);

	//StaticMesh->SetWorldScale3D(FVector(.03, .03, .03));

	// I don't want the pot to fly around.
	StaticMesh->SetMassOverrideInKg(NAME_None, 2000.f);
	HasWater = false;
	HasPasta = false;
	HasSalt = false;
	HasFire = false;
	HasPlayedSound = false;

/*	static ConstructorHelpers::FObjectFinder<USoundCue>FireAudio(TEXT("/Game/Sounds/SCue_WhereDoesFireComeFrom.SCue_WhereDoesFireComeFrom"));
	if (FireAudio.Succeeded())
	{
		FireSound = FireAudio.Object;
	}*/

 
}

void ACookingPot::BeginPlay()
{
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACookingPot::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACookingPot::OnOverlapEnd);
}


void ACookingPot::Interact()
{
	Super::Interact();
}

void ACookingPot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//GEngine->AddOnScreenDebugMessage(1, 3, FColor::Purple, FString::Printf(TEXT("Tick")));
	PlaySound();
	if (HasAnyIngridents())
	{
		if (IsTippedOver())
		{
			OnTipOver();
		}
	}

	UpdateOverlaps(true);
}

// I want to clean this up into multiple small functions.
void ACookingPot::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AIngredient* Ingredient = Cast<AIngredient>(OtherActor))
	{
		if (Ingredient->GetIsNeeded())
		{
			if (Ingredient->GetIsPasta())
			{
				HasPasta = true;
				Ingredient->HandleDestruction();
			}
			else if (Ingredient->GetIsSalt())
			{
				HasSalt = true;
				Ingredient->HandleDestruction();
			}
		}
	}
	else if (ASink* Sink = Cast<ASink>(OtherActor))
	{
		//GEngine->AddOnScreenDebugMessage(1, 3, FColor::Purple, FString::Printf(TEXT("Tick")));
		if (Sink->IsOn())
		{
			HasWater = true;
		}
	}
	else if (AStove* Stove = Cast<AStove>(OtherActor))
	{
		if (Stove->GetIsOn())
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

bool ACookingPot::HasAnyIngridents()
{
	if (HasWater || HasSalt || HasPasta)
	{
		return true;
	}

	return false;
}

bool ACookingPot::IsTippedOver()
{
	float MaxTilt = 80.f;
	float Roll = GetActorRotation().Roll;
	float Pitch = GetActorRotation().Pitch;


	if (Roll > MaxTilt || Roll < -MaxTilt)
	{
		return true;
	}
	else if (Pitch > MaxTilt || Pitch < -MaxTilt)
	{
		return true;
	}

	return false;
}

// Need to refactor this into separate functions.
void ACookingPot::OnTipOver()
{
	if (HasAnyIngridents())
	{
		FVector Scale = FVector::OneVector;
		FVector Location = IngredientSpawn->GetComponentLocation();
		FRotator Rotation = GetActorRotation();
		FTransform SpawnTransform = FTransform(Rotation, Location, Scale);
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;

		if (HasPasta)
		{
			if (PastaClass)
			{
				Pasta = GetWorld()->SpawnActor<AIngredient>(PastaClass, SpawnTransform, SpawnParams);
				HasPasta = false;
			}
		}
		if (HasSalt)
		{
			if (SaltClass)
			{
				Salt = GetWorld()->SpawnActor<AIngredient>(SaltClass, SpawnTransform, SpawnParams);
				HasSalt = false;
			}
		}
		// Need to add a water spill VFX to, but I don't thing there will be a mesh attached to this. 
	}
}

void ACookingPot::PlaySound()
{
	if (HasWater && HasSalt && HasPasta && !HasPlayedSound)
	{
		HasPlayedSound = true;
		if(FireSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), (USoundBase*)FireSound, GetActorLocation());
	}
}

bool ACookingPot::HasAllIngrediants()
{
	if (HasWater && HasSalt && HasPasta && HasFire)
	{
		//GEngine->AddOnScreenDebugMessage(1, 3, FColor::Purple, FString::Printf(TEXT("Has All Ingredients")));
		return true;
	}

	return false;
}



