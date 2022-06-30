// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/Interactable.h"
#include "Inspectable.generated.h"

/**
 * 
 */
UCLASS()
class RON_API AInspectable : public AInteractable
{
	GENERATED_BODY()

public:
	AInspectable();
	virtual void Interact() override;
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UFUNCTION()
	void StartInspection(float DeltaTime);

	UFUNCTION()
	void EndInspection(float DeltaTime);

	UFUNCTION()
	void TriggerInspection();

	UFUNCTION()
	void MoveToPlayer(float DeltaTime);

	UFUNCTION()
	void MoveToOriginalLocation(float DeltaTime);

	bool IsBeingInspected;
	bool IsStartingInspection;
	bool IsEndingInspection;
	FVector OrigialLocation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float LerpSpeed;
	float TimePassed;

	float CurrentRoll;
	float RotationChange;
	FRotator OriginalRotation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float UpRotationSpeed;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float DownRotationSpeed;
	float RotationTime;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxDegree;


};
