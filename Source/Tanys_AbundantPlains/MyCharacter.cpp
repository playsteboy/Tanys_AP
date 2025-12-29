// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Move
	bIsRunning = false;
	bIsJumping = false;
	bWasJumping = false;
	bWasRunning = false;
	bIsDashing = false;
	bWasDashing = false;
	bIsWalking = false;
	//Attack
	bIsAttacking = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//About Character Automatic Rotation

	GetCharacterMovement()->RotationRate = FRotator(0.f, 1080.f, 0.f);

	if (Stamina == 0 || bIsRunning && !bIsWalking) {
		bIsRunning = false;
	}

	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunSpeed : WalkSpeed;

	CheckStamina();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyCharacter::Walk(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();
}

void AMyCharacter::WalkEnded()
{
	bIsWalking = false;
}

void AMyCharacter::StartJump()
{
	if (Stamina >= StaminaJumpCost) {
		CanJump();
		bIsJumping = true;
		Jump();
		Stamina -= StaminaJumpCost;
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("%.2f"), Stamina));

	}
}
void AMyCharacter::StopJump()
{
	bIsJumping = false;
	StopJumping();
}

void AMyCharacter::OnToggleRun()
{
	if (!bIsRunning && Stamina > 0.0f && bIsWalking)
	{
		bIsRunning = true;

	}
	else
	{
		bIsRunning = false;
	}
}

void AMyCharacter::DecreaseStamina()
{
	if (bIsRunning && Stamina>0.0f && Stamina <= MaxStamina) {
		Stamina -= StaminaRunCost;
	}

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("%.2f"),Stamina));

}

void AMyCharacter::UpStamina()
{
	GetWorldTimerManager().ClearTimer(StaminaTimeRegeneration);
	GetWorldTimerManager().SetTimer(StaminaTimeRegeneration, this, &AMyCharacter::IncreaseStamina, 0.1f, true);
}

void AMyCharacter::IncreaseStamina()
{
	if (Stamina >= 0.0f && Stamina < MaxStamina && !bIsRunning) {
		Stamina += StaminaGain;
	}
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("%.2f"), Stamina));
}

void AMyCharacter::CheckStamina() {
	if (bIsRunning != bWasRunning || bIsJumping != bWasJumping || bIsDashing != bWasDashing)
	{
		bWasRunning = bIsRunning;
		bWasJumping = bIsJumping;
		bWasDashing = bIsDashing;

		GetWorldTimerManager().ClearTimer(StaminaTimeRegeneration);

		if (bIsRunning || bIsJumping || bIsDashing)
		{
			GetWorldTimerManager().SetTimer(StaminaTimeRegeneration, this, &AMyCharacter::DecreaseStamina, 0.5f, true);
		}
		else
		{
			GetWorldTimerManager().SetTimer(StaminaTimeRegeneration, this, &AMyCharacter::UpStamina, 10.0f, true);
		}
	}
}

void AMyCharacter::Dash() {
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		if (Stamina >= StaminaDashCost)
		{
			bIsDashing = true;
			LaunchCharacter(GetActorForwardVector() * 5000, false, true);
			Stamina -= StaminaDashCost;
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("%.2f"), Stamina));
		}
	}
}

void AMyCharacter::DashEnded() {
	bIsDashing = false;
}

void AMyCharacter::Die()
{
	AController* MyController = GetController();
	if (MyController) {
		MyController->UnPossess();
	}
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

float AMyCharacter::TakeDamage(float DamageValue, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->HealthAmount -= DamageValue;
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage called! Damage=%.2f, Health=%.2f"), DamageValue, HealthAmount);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("%.2f"), this->HealthAmount));
	if (this->HealthAmount <= 0.0f) {
		Die();
	}
	return DamageValue;
}

