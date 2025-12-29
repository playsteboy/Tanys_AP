// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "InputActionValue.h"
#include "InputMappingContext.h" 
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "GameFramework/PlayerController.h"
#include "MyCollectible.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//About default movement
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MoveAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IMC_Move.IMC_Move"));
	if (IMC_MoveAssets.Succeeded())
	{
		IMC_Move = IMC_MoveAssets.Object;
	}

	IA_Walk = CreateDefaultSubobject<UInputAction>(TEXT("Walk_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_WalkAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Walk.IA_Walk"));
	if (IA_WalkAssets.Succeeded())
	{
		IA_Walk = IA_WalkAssets.Object;
	}
	IA_Jump = CreateDefaultSubobject<UInputAction>(TEXT("Jump_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JumpAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Jump.IA_Jump"));
	if (IA_JumpAssets.Succeeded())
	{
		IA_Jump = IA_JumpAssets.Object;
	}
	IA_Run = CreateDefaultSubobject<UInputAction>(TEXT("Run_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_RunAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Run.IA_Run"));
	if (IA_RunAssets.Succeeded())
	{
		IA_Run = IA_RunAssets.Object;
	}
	IA_Dash = CreateDefaultSubobject<UInputAction>(TEXT("Dash_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DashAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Dash.IA_Dash"));
	if (IA_DashAssets.Succeeded())
	{
		IA_Dash = IA_DashAssets.Object;
	}

	//About Character Interaction
	IA_Interact = CreateDefaultSubobject<UInputAction>(TEXT("Interact_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Interact.IA_Interact"));
	if (IA_InteractAssets.Succeeded())
	{
		IA_Interact = IA_InteractAssets.Object;
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GravityScale = 3.0f;

	//About Character Rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//About Camera
	IA_Look = CreateDefaultSubobject<UInputAction>(TEXT("Look_Action"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookAssets(TEXT("/Game/Tanys_AbundantPlains/IMC/IA/IA_Look.IA_Look"));
	if (IA_LookAssets.Succeeded())
	{
		IA_Look = IA_LookAssets.Object;
	}

	//About Character Properties
	//Speed
	SpeedAmount = 1.0f;
	//Health
	HealthAmount = 100.0f;
	//Energy
	Energy = 0.0f;
	//Stamina
	Stamina = 100.0f;
	MaxStamina = 100.0f;
	StaminaRunCost = 1.0f;
	StaminaJumpCost = 10.0f;
	StaminaDashCost = 20.0f;
	StaminaGain = 1.0f;
	//Move
	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;
	//About inventory
	MaxInventorySize = 9;
	Inventory.SetNum(MaxInventorySize);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(IMC_Move, 0);
		}
	}

	//About Character Possession
	AController* MyController = GetController();
	if (MyController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character is possessed by: %s"), *MyController->GetName());
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//About Character default movement
		EnhancedInputComponent->BindAction(IA_Walk, ETriggerEvent::Triggered, this, &AMainCharacter::Walk);
		EnhancedInputComponent->BindAction(IA_Walk, ETriggerEvent::Completed, this, &AMyCharacter::WalkEnded);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharacter::StartJump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);

		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &AMyCharacter::OnToggleRun);

		EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Started, this, &AMyCharacter::Dash);
		EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Completed, this, &AMyCharacter::DashEnded);

		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &AMainCharacter::Interact);
	}
	
}

void AMainCharacter::Walk(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		bIsWalking = true;
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Input.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, Input.X);

	}
}

void AMainCharacter::Interact() {
	bool bLinearHit;
	FHitResult LinearHit;
	bLinearHit = GetWorld()->LineTraceSingleByChannel(
		LinearHit,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1000,
		ECC_Visibility
	);
	if (bLinearHit) {
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("In interaction")));
	}
}


