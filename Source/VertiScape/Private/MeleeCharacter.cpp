// Made by Alex Jobe


#include "MeleeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "VertiScape/CheckpointSystem/Checkpoint.h"
#include "VertiScape/Components/HealthComponent.h"
#include "VertiScape/Components/MeleeComponent.h"

//////////////////////////////////////////////////////////////////////////
// AMeleeCharacter

AMeleeCharacter::AMeleeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Melee Combat
	MeleeComp = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeCombat"));
	MeleeComp->SetupAttachment(RootComponent);

	// Create health component
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	bIsDead = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Save System

FString AMeleeCharacter::GetLastCheckpointName()
{
	return LastCheckpointName;
}

void AMeleeCharacter::SetLastCheckpointName(FString NewCheckpointName)
{
	LastCheckpointName = NewCheckpointName;
}

FSavableData AMeleeCharacter::SaveData()
{
	FSavableData SavableData;
	SavableData.Transform = GetActorTransform();
	SavableData.LastCheckpointName = LastCheckpointName;
	return SavableData;
}

void AMeleeCharacter::LoadData(FSavableData DataToLoad)
{
	SetActorTransform(DataToLoad.Transform);
	LastCheckpointName = DataToLoad.LastCheckpointName;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMeleeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMeleeCharacter::BeginMeleeAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMeleeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMeleeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMeleeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMeleeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMeleeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMeleeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMeleeCharacter::OnResetVR);
}

void AMeleeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &AMeleeCharacter::OnHealthChanged);
	}
}

void AMeleeCharacter::OnHealthChanged(class UHealthComponent* HealthComponent, float Health, float HealthChangeAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f && !bIsDead)
	{
		// Time to die
		bIsDead = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorHiddenInGame(true);
	}
}

void AMeleeCharacter::BeginMeleeAttack()
{
	check(MeleeComp)
	MeleeComp->BeginAttack();
}

void AMeleeCharacter::OnResetVR()
{
	// If VertiScape is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in VertiScape.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMeleeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMeleeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMeleeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMeleeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMeleeCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMeleeCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}