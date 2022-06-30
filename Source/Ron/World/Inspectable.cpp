// Fill out your copyright notice in the Description page of Project Settings.


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
	// The lower the number, the faster it moves. Down rotation needs to be slow so the down location can finish before the rotaion;
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

	// Smooth Rotation
	if (FMath::IsNearlyEqual(CurrentRoll, MaxDegree, 10.5f))
	{
		IsEndingInspection = false;
		IsStartingInspection = false;
		IsBeingInspected = true;
		UE_LOG(LogTemp, Warning, TEXT("Start"));
	}
	else if (IsStartingInspection && RotationTime < UpRotationSpeed)
	{
		// RInterpTo was having issues so we use lerp range.
		SetActorRotation(FMath::LerpRange(OriginalRotation, FRotator(0, 0, MaxDegree), (RotationTime / UpRotationSpeed)));
		RotationTime += DeltaTime;
	}
}

void AInspectable::EndInspection(float DeltaTime)
{
	IsEndingInspection = true;
	MoveToOriginalLocation(DeltaTime);

	CurrentRoll = StaticMesh->GetRelativeRotation().Roll;

	// Smooth Rotation
	if (FMath::IsNearlyEqual(CurrentRoll, 0, 1.2f))
	{
		IsStartingInspection = false;
		IsBeingInspected = false;
		IsEndingInspection = false;
		UE_LOG(LogTemp, Warning, TEXT("End"));
	}
	else if (IsEndingInspection && RotationTime < DownRotationSpeed)
	{
		// RInterpTo was having issues so we use lerp range.
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
				// Smoothly move to the players hold location.
				if (TimePassed < LerpSpeed)
				{
					SetActorLocation(FMath::Lerp(OrigialLocation, Player->GetHoldLocation(), (TimePassed / LerpSpeed)));
					TimePassed += DeltaTime;
				}
				//TimePassed = 0.f;
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
				// Smoothly move to the original location.
				if (TimePassed < LerpSpeed)
				{
					SetActorLocation(FMath::Lerp(Player->GetHoldLocation(), OrigialLocation, (TimePassed / LerpSpeed)));
					TimePassed += DeltaTime;
				}
				//TimePassed = 0.f;
		}
	}
}


