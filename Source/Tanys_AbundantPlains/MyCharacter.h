// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MyCharacter.generated.h"
UCLASS()
class TANYS_ABUNDANTPLAINS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Speed
	
	float SpeedAmount;
	//Health
	float HealthAmount;
	//Energy
	float Energy;
	//Stamina
	float Stamina;
	float StaminaRunCost;
	float StaminaJumpCost;
	float StaminaDashCost;
	float StaminaGain;
	float MaxStamina; //value of the max stamina
	//Move
	float WalkSpeed;
	float RunSpeed;
	bool bIsJumping; //verifying if the character is jumping
	bool bIsRunning; //verifying if the character is running
	bool bWasRunning; //verifying if the character was running before 
	bool bWasJumping;
	bool bWasDashing;
	bool bIsDashing;
	bool bIsWalking;

	//Attack
	bool bIsAttacking;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//About movement
	virtual void Walk(const FInputActionValue& Value);
	virtual void WalkEnded();
	virtual void StartJump();
	virtual void StopJump();
	virtual void OnToggleRun();
	virtual void Dash();
	virtual void DashEnded();

	//About stamina
	virtual void UpStamina();
	virtual void DecreaseStamina();
	virtual void IncreaseStamina();
	virtual void CheckStamina();
	FTimerHandle StaminaTimeRegeneration;
	//About health
	virtual void Die();
	virtual float GetHealthAmount() { return HealthAmount; }

	//About Attack
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool GetIsAttacking() const { return bIsAttacking; }
	virtual void SetIsAttacking(bool value) { bIsAttacking = value; };
};
