// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ron/World/Interactable.h"
#include "WorldPickup.generated.h"

UCLASS()
class RON_API AWorldPickup : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AWorldPickup();

	UStaticMeshComponent* GetMeshStaticMesh() { return StaticMesh; }
	bool IsLargePickup() { return IsLarge; }

	virtual void Interact() override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	bool IsBeingHeld;

	UPROPERTY()
	bool IsLarge;
private:


};
