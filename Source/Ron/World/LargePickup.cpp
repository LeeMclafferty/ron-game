// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/LargePickup.h"
#include "Components/StaticMeshComponent.h"
#include <Runtime/Core/Public/ProfilingDebugging/CookStats.h>

ALargePickup::ALargePickup()
:ShowDebugLine(false), Floor(0.f)
{
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetMassOverrideInKg(NAME_None, 9999.f, true);
	
	IsLarge = true;
}

void ALargePickup::Interact()
{
	Super::Interact();

	if (!IsBeingHeld)
	{
		StaticMesh->SetSimulatePhysics(true);
		//StaticMesh->SetCollisionProfileName("IgnoreOnlyPawn");
	}
}

void ALargePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FindFloor();
	MoveToFloor();
}

bool ALargePickup::FindFloor()
{

	FVector StartLocation = GetActorLocation();
	//FRotator Rotation = GetActorUpVector().Rotation() * -1;
	FVector EndLocation =  GetActorLocation().Z * FVector::DownVector * 200;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (ShowDebugLine)
	{
		// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1, 0, 10);
		DrawDebugLine(GetWorld(), StartLocation, EndLocation , FColor::Red, false, -1.f, 0);
	}

	return GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);

}

void ALargePickup::MoveToFloor()
{
	if (!Hit.GetActor())
	{
		return;
	}
	AActor* HitActor = Hit.GetActor();
	Floor = Hit.ImpactPoint.Z + 20; //float right above the floor so there is no collision issues.
	FVector NewLocation(GetActorLocation().X, GetActorLocation().Y, Floor);
	//SetActorLocation(NewLocation);
}

void ALargePickup::Drag(float X, float Y)
{
	SetActorLocation(FVector(X, Y, Floor));
}