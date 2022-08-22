 
#include "Ron/Player/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Ron/World/WorldPickup.h"
#include "Ron/World/Lootable.h"
#include "Ron/Items/ItemData.h"
#include "Ron/Framework/KitchenGameMode.h"
#include "Ron/Player/RonController.h"
#include "Ron/World/LargePickup.h"

ACharacterBase::ACharacterBase()
:ShowDebugPoint(false), ReachRange(100.f), ReachMin(50.f), ReachMax(100.f), HasItemHeld(false)
{
	PrimaryActorTick.bCanEverTick = true;

	HoldLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Item Hold Location"));
	HoldLocation->SetupAttachment(RootComponent);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetMovementComponent()->SetActive(true);
	SetKitchenGameMode();
	SetRonController();
}

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ACharacterBase::Interact);
	PlayerInputComponent->BindAction("DecreaseReach", IE_Pressed, this, &ACharacterBase::DecreaseReach);
	PlayerInputComponent->BindAction("IncreaseReach", IE_Pressed, this, &ACharacterBase::IncreaseReach);
	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &ACharacterBase::OpenPauseMenu);
}

bool ACharacterBase::LookTrace(FHitResult& Hit, FVector& LookDirection)
{
	AController* PlayerController = GetController();

	if (PlayerController)
	{
		FVector StartLocation;
		FRotator Rotation;
		PlayerController->GetPlayerViewPoint(StartLocation, Rotation);
		FVector EndLocation = StartLocation + Rotation.Vector() * ReachRange;
		LookDirection = -Rotation.Vector();

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (AInteractable* Actor = Cast<AInteractable>(Hit.GetActor()))
		{
			LookedAtActor = Cast<AInteractable>(Hit.GetActor());
		}
		else
		{
			LookedAtActor = nullptr;
		}

		if (ShowDebugPoint)
		{
			// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1, 0, 10);
			DrawDebugPoint(GetWorld(), Hit.Location, 50.f, FColor::Red, false, -1.f, 0);
		}

		return GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);

	}

	LookedAtActor = nullptr;
	return false;
}

void ACharacterBase::SetKitchenGameMode()
{
	if (auto CurrentGameMode = Cast<AKitchenGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		KitchenGameMode = CurrentGameMode;
	}
}

void ACharacterBase::SetRonController()
{
	if (auto CurrentController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		RonController = CurrentController;
	}
}

void ACharacterBase::Interact()
{
	if (LookedAtActor)
	{
		LookedAtActor->Interact();
	}
	else if (HeldActor)
	{
		HeldActor->Interact();
	}
}

void ACharacterBase::DecreaseReach()
{
	if (HasItemHeld)
	{
		if (ReachRange > ReachMin)
		{
			ReachRange -= 10;
		}
		else
		{
			ReachRange = ReachMin;
		}
	}
}

void ACharacterBase::IncreaseReach()
{
	if (HasItemHeld)
	{
		if (ReachRange < ReachMax)
		{
			ReachRange += 10;
		}
		else
		{
			ReachRange = ReachMax;
		}
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookTrace(OutHit, OutDirection);
	//UE_LOG(LogTemp, Warning, TEXT("HandDestruction"))
	
	if (HeldActor && HasItemHeld)
	{	
		if (!HeldActor->IsLargePickup())
		{
			UpdateHeldActorLoction();
		}
		else
		{
			DragHeldActor();
		}
	}
	else
	{
		ReachRange = ReachMax;
	}
}

void ACharacterBase::UpdateHeldActorLoction()
{

	if (AController* PlayerController = GetController())
	{
		FVector Loc;
		FRotator Rot;
		PlayerController->GetPlayerViewPoint(Loc, Rot);

		if (HeldActor && HasItemHeld) // If the actor is destroyed I want to reject the ptr is still valid.
		{
			HeldActor->SetActorLocation(Loc + Rot.Vector() * ReachRange, true);
			HeldActor->SetActorRotation(FRotator::ZeroRotator);
			//GEngine->AddOnScreenDebugMessage(-1, .5, FColor::Purple, FString::Printf(TEXT("Held Actor: %s"), *HeldActor->GetName()));
		}
	}
}

void ACharacterBase::DragHeldActor()
{
	if (!HeldActor)
	{
		return;
	}
	else if(ALargePickup* Large= Cast<ALargePickup>(HeldActor))
	{
		if (AController* PlayerController = GetController())
		{
			FVector Loc;
			FRotator Rot;
			PlayerController->GetPlayerViewPoint(Loc, Rot);
			if (HeldActor) 
			{
				ReachRange = 150.f;
				Large->Drag(Loc.X + Rot.Vector().X * ReachRange, Loc.Y + Rot.Vector().Y * ReachRange);
				//Large->SetActorLocation(FVector(Loc.X + Rot.Vector().X * ReachRange, Loc.Y + Rot.Vector().Y * ReachRange, 191.f));
				Large->SetActorRotation(HoldLocation->GetComponentRotation());
			}
			//GEngine->AddOnScreenDebugMessage(-1, .5, FColor::Purple, FString::Printf(TEXT("Held Actor: %s"), *HeldActor->GetName()));
		}
	}

}

void ACharacterBase::OpenPauseMenu()
{
	//UE_LOG(LogTemp, Warning, TEXT("open"))
	if (KitchenGameMode && RonController)
	{
		RonController->PauseGame();
	}
}




