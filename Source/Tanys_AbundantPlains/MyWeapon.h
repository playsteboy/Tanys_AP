// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class TANYS_ABUNDANTPLAINS_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float DamageAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AMyCharacter* Character;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void WeaponAttack(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
