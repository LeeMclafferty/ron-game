// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ron/World/WorldPickup.h"
#include "CookingPot.generated.h"


UCLASS(Blueprintable)
class RON_API ACookingPot : public AWorldPickup
{
	GENERATED_BODY()

public:

	ACookingPot();
	virtual void Interact() override;
	virtual void Tick(float DeltaSeconds) override;

	bool HasAllIngrediants();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	bool HasWater;
	UPROPERTY(VisibleAnywhere)
	bool HasPasta;
	UPROPERTY(VisibleAnywhere)
	bool HasSalt;
	UPROPERTY(VisibleAnywhere)
	bool HasFire;

	bool HasAnyIngridents();
	bool IsTippedOver();
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* IngredientSpawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIngredient> SaltClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIngredient> PastaClass;

	class AIngredient* Salt;
	class AIngredient* Pasta;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTipOver();
};
