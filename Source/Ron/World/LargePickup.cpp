// Fill out your copyright notice in the Description page of Project Settings.


#include "Ron/World/LargePickup.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Ron/Player/CharacterBase.h"

ALargePickup::ALargePickup()
:ShowDebugLine(false), Floor(0.f), DragHeight(20.2), Player(nullptr), InterpSpeed(5.f), SpeedWhileDragging(85.f)
{
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetLinearDamping(10.f);
	StaticMesh->SetAngularDamping(10.f);
	
	
	IsLarge = true;
}

void ALargePickup::Interact()
{
	//Super::Interact();
	if (!IsBeingHeld)
	{
		OnDragStart();
	}
	else
	{
		OnDragStop();
	}

}

void ALargePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);;
	FindFloor();
	MoveToFloor();
}

void ALargePickup::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetMassOverrideInKg(NAME_None, 9999.f, true);
	Player = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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

	return GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_GameTraceChannel2, Params);

}

void ALargePickup::MoveToFloor()
{
	if (!Hit.GetActor())
	{
		return;
	}
	AActor* HitActor = Hit.GetActor();
	Floor = Hit.ImpactPoint.Z + DragHeight; //float right above the floor so there is no collision issues.
	//SetActorLocation(NewLocation);
}


void ALargePickup::Drag(float X, float Y)
{
	FVector CurrentLocation(GetActorLocation());
	FVector TargetLocation(X, Y, Floor);
	FVector Interp = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	SetActorLocation(Interp, true);
}

void ALargePickup::OnDragStart()
{
	if (!Player || !PC)
		return;
	Player->SetHoldingItem(true);
	Player->SetHeldActor(this);
	Player->GetCharacterMovement()->MaxWalkSpeed = SpeedWhileDragging;
	PC->PlayerInput->SetMouseSensitivity(.02);
	IsBeingHeld = true;
	StaticMesh->CanCharacterStepUpOn = ECB_No;
}

void ALargePickup::OnDragStop()
{
	if (!Player || !PC)
		return;
	Player->SetHoldingItem(false);
	Player->SetHeldActor(nullptr);
	Player->GetCharacterMovement()->MaxWalkSpeed = 175;
	PC->PlayerInput->SetMouseSensitivity(.04);
	IsBeingHeld = false;
	StaticMesh->CanCharacterStepUpOn = ECB_Yes;
}