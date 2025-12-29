// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MainCharacter.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "SpawnZone.h"

AMyPlayerController::AMyPlayerController()
{
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MoveAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IMC_Move.IMC_Move"));
    if (IMC_MoveAssets.Succeeded())
    {
        IMC_Move = IMC_MoveAssets.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_InteractionAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IMC_Interaction.IMC_Interaction"));
    if (IMC_InteractionAssets.Succeeded())
    {
        IMC_Interaction = IMC_InteractionAssets.Object;
    }
    else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Input Mapping Context"));
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> IA_MouseLeftClickAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_MouseLeftClick.IA_MouseLeftClick"));
    if (IA_MouseLeftClickAssets.Succeeded())
    {
        IA_MouseLeftClick = IA_MouseLeftClickAssets.Object;
    }
    bIsSpawned = false;

}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
    {
        InputSubsystem->AddMappingContext(IMC_Move, 0);
        InputSubsystem->AddMappingContext(IMC_Interaction, 1);
    }

    SpawnZone = Cast<ASpawnZone>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnZone::StaticClass()));
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(IA_MouseLeftClick, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleLeftClick);
    }
}


void AMyPlayerController::HandleLeftClick(const FInputActionInstance& Instance)
{
    FVector2D MousePosition;
    if (GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        FVector WorldOrigin, WorldDirection;
        if (DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldOrigin, WorldDirection))
        {
            FHitResult Hit;
            FVector TraceStart = WorldOrigin;
            FVector TraceEnd = TraceStart + WorldDirection * 10000.0f;

            FCollisionQueryParams Params;
            Params.bTraceComplex = true;

            if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
            {
                FVector MySpawnLocation = Hit.ImpactPoint;

                if (CharacterToSpawn && !bIsSpawned)
                {
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                    float CapsuleHalfHeight = CharacterToSpawn->GetDefaultObject<AMainCharacter>()
                        ->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

                    MySpawnLocation.Z += CapsuleHalfHeight;
                    AMainCharacter* MyChar = GetWorld()->SpawnActor<AMainCharacter>(
                        CharacterToSpawn,
                        MySpawnLocation,
                        FRotator::ZeroRotator,
                        SpawnParams
                    );
                    if (MyChar)
                    {
                        Possess(MyChar);
						SpawnZone->ActivateSpawnZone();
                        bIsSpawned = true;
                    }
                }
            }
        }
    }
}
