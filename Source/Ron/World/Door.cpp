// Fill out your copyright notice in the Description page of Project Settings.

#include "Ron/World/Door.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"

#include "Ron/World/Key.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(RootComponent);

	UnlockTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Unlock Trigger"));
	UnlockTrigger->SetupAttachment(DoorMesh);

	IsUnlocked = true;
	DoorIsOpen = false;
	IsClosing = false;
	IsOpening = false;

	MaxDegree = 90.f;
	AddRotation = 0.f;
	DoorCurrentRotation = 0.f;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	UnlockTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpening)
	{
		OpenDoor(DeltaTime);
	}
	if (IsClosing)
	{
		CloseDoor(DeltaTime);
	}
}

void ADoor::Interact()
{
	Super::Interact();

	ActivateDoor();
}

void ADoor::OpenDoor(float DeltaTime)
{
	DoorCurrentRotation = DoorMesh->GetRelativeRotation().Yaw;

	AddRotation = DeltaTime * 80;

	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		IsClosing = false;
		IsOpening = false;
	}
	else if (IsOpening) 
	{
		FRotator NewRotation = FRotator(0.f, AddRotation, 0.0f);
		DoorMesh->AddRelativeRotation(FQuat(NewRotation), false);
	}
}

void ADoor::CloseDoor(float DeltaTime)
{
	DoorCurrentRotation = DoorMesh->GetRelativeRotation().Yaw;

	if (DoorCurrentRotation > 0)
	{
		AddRotation = -DeltaTime * 80;
	}
	else
	{
		AddRotation = DeltaTime* 80;
	}

	if (FMath::IsNearlyEqual(DoorCurrentRotation, 0.f, 1.5f))
	{
		IsOpening = false;
		IsClosing = false;
	}
	else if (IsClosing)
	{
		FRotator NewRotation = FRotator(0.f, AddRotation, 0.f);
		DoorMesh->AddRelativeRotation(FQuat(NewRotation), false);
	}
}

void ADoor::ActivateDoor()
{
	if (!DoorIsOpen)
	{
		if (IsUnlocked)
		{
			IsClosing = false;
			IsOpening = true;
			DoorIsOpen = true;
			UE_LOG(LogTemp, Warning, TEXT("Open"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Door Locked"));
		}
	}
	else
	{
		IsClosing = true;
		IsOpening = false;
		DoorIsOpen = false;
	}
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUnlockKey* DoorKey = Cast<AUnlockKey>(OtherActor))
	{
		if (DoorKey->GetTriggerBox())
		{
			if (!IsUnlocked && UnlockKey)
			{
				if (UnlockKey == DoorKey)
				{
					IsUnlocked = true;
					UnlockKey->DisableKey();
				}
			}
		}
	}
}

void ADoor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

