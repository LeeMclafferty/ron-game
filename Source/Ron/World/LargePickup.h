// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/WorldPickup.h"
#include "LargePickup.generated.h"

UCLASS()
class RON_API ALargePickup : public AWorldPickup
{
	GENERATED_BODY()

public:

	ALargePickup();

	virtual void Interact() override;
	void Drag(float X, float Y);

protected:

	virtual void Tick(float DeltaSeconds) override;

private:

	bool FindFloor();
	void MoveToFloor();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	bool ShowDebugLine;

	FHitResult Hit;
	float Floor;
	
};
