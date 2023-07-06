#include "Ron/World/Inspectable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "Ron/Player/CharacterBase.h"
#include "Ron/Player/RonController.h"

AInspectable::AInspectable()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;
	MaxDegree = -90;
	LerpSpeed = 3.f;
	TimePassed = 0.f;
	RotationTime = 0.f;
	UpRotationSpeed = .5f;
	DownRotationSpeed = 5.f;

	StaticMesh->SetCollisionProfileName("IgnoreOnlyPawn");
}

void AInspectable::BeginPlay()
{
	Super::BeginPlay();

	OrigialLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();
}

void AInspectable::Tick(float DeltaTime)
{
	if (IsStartingInspection)
	{
		StartInspection(DeltaTime);
	}
	if (IsEndingInspection)
	{
		EndInspection(DeltaTime);
	}
}

void AInspectable::Interact()
{
	Super::Interact();

	TimePassed = 0.f;
	RotationTime = 0.f;
	TriggerInspection();
}

void AInspectable::StartInspection(float DeltaTime)
{
	IsStartingInspection = true;
	MoveToPlayer(DeltaTime);

	CurrentRoll = StaticMesh->GetRelativeRotation().Roll;

	if (FMath::IsNearlyEqual(CurrentRoll, MaxDegree, 10.5f))
	{
		IsEndingInspection = false;
		IsStartingInspection = false;
		IsBeingInspected = true;
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}
	else if (IsStartingInspection && RotationTime < UpRotationSpeed)
	{
		SetActorRotation(FMath::LerpRange(OriginalRotation, FRotator(0, 0, MaxDegree), (RotationTime / UpRotationSpeed)));
		RotationTime += DeltaTime;
	}
}

void AInspectable::EndInspection(float DeltaTime)
{
	IsEndingInspection = true;
	MoveToOriginalLocation(DeltaTime);

	CurrentRoll = StaticMesh->GetRelativeRotation().Roll;

	if (FMath::IsNearlyEqual(CurrentRoll, 0, 1.2f))
	{
		IsStartingInspection = false;
		IsBeingInspected = false;
		IsEndingInspection = false;
		UE_LOG(LogTemp, Warning, TEXT("End"));
	}
	else if (IsEndingInspection && RotationTime < DownRotationSpeed)
	{
		SetActorRotation(FMath::LerpRange(GetActorRotation(), OriginalRotation, (RotationTime / DownRotationSpeed)));
		RotationTime += DeltaTime;
	}
}

void AInspectable::TriggerInspection()
{
	if (!IsBeingInspected)
	{
		IsEndingInspection = false;
		IsStartingInspection = true;
		IsBeingInspected = false;
	}
	else
	{
		IsEndingInspection = true;
		IsStartingInspection = false;
		IsBeingInspected = false;
	}
}

void AInspectable::MoveToPlayer(float DeltaTime)
{
	if (ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (ARonController* Controller = Cast<ARonController>(Player->GetController()))
		{
			if (IsStartingInspection)
			{
				Player->GetMovementComponent()->SetActive(false);

				if (TimePassed < LerpSpeed)
				{
					SetActorLocation(FMath::Lerp(OrigialLocation, Player->GetHoldLocation(), (TimePassed / LerpSpeed)));
					TimePassed += DeltaTime;
				}
			}
		}
	}
}

void AInspectable::MoveToOriginalLocation(float DeltaTime)
{
	if (ACharacterBase* Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (ARonController* Controller = Cast<ARonController>(Player->GetController()))
		{
			Player->GetMovementComponent()->SetActive(true);

			if (TimePassed < LerpSpeed)
			{
				SetActorLocation(FMath::Lerp(Player->GetHoldLocation(), OrigialLocation, (TimePassed / LerpSpeed)));
				TimePassed += DeltaTime;
			}
		}
	}
}
