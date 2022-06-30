// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/Interactable.h"
#include "Sink.generated.h"

/**
 * 
 */
UCLASS()
class RON_API ASink : public AInteractable
{
	GENERATED_BODY()

public:

	ASink();
	virtual void Interact() override;
	class UCapsuleComponent* GetWaterPath() { return WaterPath; }
	bool IsOn() { return TurnedOn; }

protected:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

private:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* WaterPath;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HandleTrigger;

	UPROPERTY(VisibleAnywhere)
	bool TurnedOn;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WaterMesh;
	
};
