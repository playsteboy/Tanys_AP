// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyCharacter.h"
class UInputMappingContext; class UInputAction; struct FInputActionInstance; class UDecalComponent; class ASpawnZone;
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"
/**
 * 
 */
UCLASS()
class TANYS_ABUNDANTPLAINS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION()
    void HandleLeftClick(const FInputActionInstance& Instance);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsSpawned;

    AMyCharacter* Character;

    ASpawnZone* SpawnZone;

public:
	AMyPlayerController();


public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* IMC_Interaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* IA_MouseLeftClick;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* IMC_Move;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Classes")
    TSubclassOf<ACharacter> CharacterToSpawn;

    UFUNCTION(BlueprintCallable)
    bool GetIsSpawned() const {
        return bIsSpawned;
	}
};
