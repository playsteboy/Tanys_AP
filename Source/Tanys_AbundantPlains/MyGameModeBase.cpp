// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MyPlayerController.h"
#include "Engine/World.h"
#include "Engine/Level.h"

AMyGameModeBase::AMyGameModeBase()
{
    static ConstructorHelpers::FClassFinder<ACharacter> PlayerPawnClassFinder(TEXT("/Game/Tanys_AbundantPlains/Blueprints/Characters/BP_MainCharacter"));
    if (PlayerPawnClassFinder.Succeeded())
    {
        DefaultPawnClass = PlayerPawnClassFinder.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character not found"));
    }
    bStartPlayersAsSpectators = true;

	static ConstructorHelpers::FClassFinder<AMyPlayerController> PlayerControllerClassFinder(TEXT("/Game/Tanys_AbundantPlains/Blueprints/PlayerControllers/BP_MyPlayerController"));
    if(PlayerControllerClassFinder.Succeeded())
    {
        PlayerControllerClass = PlayerControllerClassFinder.Class;
        UE_LOG(LogTemp, Error, TEXT("MyPlayerController found"));
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("MyPlayerController not found"));
    }
}


