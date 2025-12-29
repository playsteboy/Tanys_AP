// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class AMyPlayerController; class USphereComponent; class AMainCharacter;
#include "CoreMinimal.h"
#include "MyZone.h"
#include "SpawnZone.generated.h"

/**
 * 
 */
UCLASS()
class TANYS_ABUNDANTPLAINS_API ASpawnZone : public AMyZone
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	float SpawnRadius;
	float SpawnDamageAmount;
	AMyPlayerController* MyPlayerController;
	AMainCharacter* MainCharacter;
public:
	// Sets default values for this actor's properties
	ASpawnZone();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetSpawnRadius() const { return SpawnRadius; }

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;

	UFUNCTION(BlueprintCallable)
	float GetSpawnDamageAmount() const { return SpawnDamageAmount; }

	UFUNCTION()
	void ActivateSpawnZone();

};
