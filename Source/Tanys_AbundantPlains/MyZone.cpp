// Fill out your copyright notice in the Description page of Project Settings.


#include "MyZone.h"

// Sets default values
AMyZone::AMyZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        return;
    }

    FVector WorldLocation;
    FVector WorldDirection;

    if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
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

