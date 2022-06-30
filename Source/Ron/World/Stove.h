// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/Interactable.h"
#include "Stove.generated.h"

/**
 * 
 */
UCLASS()
class RON_API AStove : public AInteractable
{
	GENERATED_BODY()

public:
	AStove();

	virtual void Interact() override;
	bool GetIsOn() { return IsTurnedOn; }

protected:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	bool IsTurnedOn;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* FireMesh;
private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BurnerBox;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ButtonBox;

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* BurnerLight;

	
};
