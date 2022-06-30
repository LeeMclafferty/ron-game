// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/WorldPickup.h"
#include "Key.generated.h"

/**
 * 
 */
UCLASS()
class RON_API AUnlockKey : public AWorldPickup
{
	GENERATED_BODY()


public:
	AUnlockKey();

	class UBoxComponent* GetTriggerBox() { return Trigger; }

	void DisableKey();

	virtual void Interact() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;
};
