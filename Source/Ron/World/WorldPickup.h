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

	virtual void Interact() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

private:

	UPROPERTY(VisibleAnywhere)
	bool IsBeingHeld;

};
