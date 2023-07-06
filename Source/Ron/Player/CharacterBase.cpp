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
    : ShowDebugPoint(false), ReachRange(100.f), ReachMin(50.f), ReachMax(100.f), HasItemHeld(false)
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
            LookedAtActor = Actor;
        }
        else
        {
            LookedAtActor = nullptr;
        }

        if (ShowDebugPoint)
        {
            DrawDebugPoint(GetWorld(), Hit.Location, 50.f, FColor::Red, false, -1.f, 0);
        }

        return GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);
    }

    LookedAtActor = nullptr;
    return false;
}

void ACharacterBase::SetKitchenGameMode()
{
    AKitchenGameMode* CurrentGameMode = Cast<AKitchenGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (CurrentGameMode)
    {
        KitchenGameMode = CurrentGameMode;
    }
}

void ACharacterBase::SetRonController()
{
    ARonController* CurrentController = Cast<ARonController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (CurrentController)
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

    FVector LookDirection;
    LookTrace(OutHit, LookDirection);

    if (HeldActor && HasItemHeld)
    {
        if (!HeldActor->IsLargePickup())
        {
            UpdateHeldActorLocation();
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

void ACharacterBase::UpdateHeldActorLocation()
{
    AController* PlayerController = GetController();
    if (PlayerController && HeldActor && HasItemHeld)
    {
        FVector Loc;
        FRotator Rot;
        PlayerController->GetPlayerViewPoint(Loc, Rot);

        HeldActor->SetActorLocation(Loc + Rot.Vector() * ReachRange, true);
        HeldActor->SetActorRotation(FRotator::ZeroRotator);
    }
}

void ACharacterBase::DragHeldActor()
{
    if (!HeldActor)
    {
        return;
    }
    else if (ALargePickup* Large = Cast<ALargePickup>(HeldActor))
    {
        AController* PlayerController = GetController();
        if (PlayerController && HeldActor)
        {
            FVector Loc;
            FRotator Rot;
            PlayerController->GetPlayerViewPoint(Loc, Rot);
            ReachRange = 150.f;
            Large->Drag(Loc.X + Rot.Vector().X * ReachRange, Loc.Y + Rot.Vector().Y * ReachRange);
            Large->SetActorRotation(HoldLocation->GetComponentRotation());
        }
    }
}

void ACharacterBase::OpenPauseMenu()
{
    if (KitchenGameMode && RonController)
    {
        RonController->PauseGame();
    }
}
