// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ron/World/Interactable.h"
#include "Door.generated.h"

UCLASS()
class RON_API ADoor : public AInteractable
{
	GENERATED_BODY()
	
public:	
	ADoor();
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact() override;

	void SetUnlockKey(class AUnlockKey* InKey) { UnlockKey = InKey; }
	void LockDoor() { IsUnlocked = false; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool IsUnlocked;

	UPROPERTY(VisibleAnywhere)
	bool DoorIsOpen;

	bool IsClosing;
	bool IsOpening;

	// Set this in editor per door
	UPROPERTY(EditAnywhere)
	class AUnlockKey* UnlockKey;

private:	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* UnlockTrigger;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void ActivateDoor();

	float MaxDegree;
	float AddRotation;
	float DoorCurrentRotation;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
