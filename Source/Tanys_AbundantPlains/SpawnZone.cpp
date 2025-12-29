// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnZone.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"


ASpawnZone::ASpawnZone()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	SpawnRadius = 250.0f;
	SpawnDamageAmount = 20.0f;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(SpawnRadius*2);
    RootComponent = CollisionComponent;
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);

}

void ASpawnZone::BeginPlay()
{
    Super::BeginPlay();
    MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    
}

void ASpawnZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MyPlayerController)
    {
        return;
    }

    if (!MyPlayerController->GetIsSpawned())
    {
        FVector WorldLocation;
        FVector WorldDirection;

        if (MyPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
        {
            FHitResult HitResult;
            FVector StartLocation = WorldLocation;
            FVector EndLocation = StartLocation + (WorldDirection * 10000.0f);
            FCollisionQueryParams CollisionParams;
            CollisionParams.AddIgnoredActor(this);

            if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
            {
                FVector TargetLocation = HitResult.ImpactPoint;
                SetActorLocation(TargetLocation);
            }
        }
    }
}

void ASpawnZone::ActivateSpawnZone()
{
    if (!MyPlayerController)
    {
        return;
    }
    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor != this && Actor != MainCharacter)
        {
            if (AEnemy* Enemy = Cast<AEnemy>(Actor)) {
                UGameplayStatics::ApplyDamage(Enemy, SpawnDamageAmount, MyPlayerController, this, UDamageType::StaticClass());
                UE_LOG(LogTemp, Warning, TEXT("Overlapping actors: %d"), OverlappingActors.Num());

            }
        }
    }
}
