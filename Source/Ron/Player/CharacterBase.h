// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ron/RonCharacter.h"
#include "CharacterBase.generated.h"

UCLASS()
class RON_API ACharacterBase : public ARonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHeldActor(class AWorldPickup* InActor) { HeldActor = InActor; }
	void SetHoldingItem(bool IsHolding) { HasItemHeld = IsHolding; }
	void SetActorBeingLooted(class ALootable* Lootable) { ActorBeingLooted = Lootable; }

	FVector GetHoldLocation() const { return HoldLocation->GetComponentLocation(); } 

	UFUNCTION(BlueprintCallable)
	class ALootable* GetActorBeingLooted() const{ return ActorBeingLooted; }

	UFUNCTION(BlueprintCallable)
	bool CanGrab() const { return LookedAtActor ? true : false; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* HoldLocation;
	 
private:
	// Out params as arguments
	bool LookTrace(FHitResult& Hit, FVector& LookDirection);

	//Setup cast in begin play
	void SetKitchenGameMode();
	void SetRonController();
	class AKitchenGameMode* KitchenGameMode;
	class ARonController* RonController;

	FHitResult OutHit;
	FVector OutDirection;

	UPROPERTY(EditAnywhere)
	bool ShowDebugPoint;

	UPROPERTY(EditAnywhere)
	float ReachRange;
	UPROPERTY(EditAnywhere)
	float ReachMin;
	UPROPERTY(EditAnywhere)
	float ReachMax;

	class AInteractable* LookedAtActor = nullptr;
	class AWorldPickup* HeldActor = nullptr;

	bool HasItemHeld;

	UFUNCTION()
	void Interact();
	UFUNCTION()
	void DecreaseReach();
	UFUNCTION()
	void IncreaseReach();
	UFUNCTION()
	void UpdateHeldActorLoction();
	UFUNCTION()
	void DragHeldActor();

	UFUNCTION()
	void OpenPauseMenu();

	class ALootable* ActorBeingLooted;

};
