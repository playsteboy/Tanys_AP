// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::WeaponAttack(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (Character->GetIsAttacking() == false && OtherActor && OtherActor != this)
	{
		// Apply Damage
		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageAmount,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
		Character->SetIsAttacking(true);
	}
}