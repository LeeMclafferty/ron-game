// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/LargePickup.h"
#include "Components/StaticMeshComponent.h"

ALargePickup::ALargePickup()
{
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetMassOverrideInKg(NAME_None, 9999.f, true);
}

/* Need to override the interact function to not diable physics, I want more of a drag than a
pickup for these.*/
