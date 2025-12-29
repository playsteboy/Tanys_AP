// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class UInputComponent; class UInputMappingContext; class UInputAction; class UEnhancedInputComponent; struct FInputActionValue; class AMyCollectible; struct FMyInventorySlot; class AMyCollectible;
#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TANYS_ABUNDANTPLAINS_API AMainCharacter : public AMyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Walk;

	UPROPERTY()
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_Interact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//About Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FMyInventorySlot> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySize;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Walk(const FInputActionValue& Value) override;

	//About interaction
	void Interact();
};
