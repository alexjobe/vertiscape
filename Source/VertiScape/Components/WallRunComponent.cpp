// Made by Alex Jobe


#include "WallRunComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UWallRunComponent::UWallRunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetAutoActivate(true);

	InputAxisRight = "MoveRight";
	InputAxisForward = "MoveForward";

	MaxJumps = 2;

	WallRunGravityScale = 0.f;
	WallRunAirControl = 1.f;

	WallRunLean = 20.f;
	WallRunLeanSpeed = 10.f;
}


// Called when the game starts
void UWallRunComponent::BeginPlay()
{
	Super::BeginPlay();

	JumpsLeft = MaxJumps;

	OwningCharacter = Cast<ACharacter>(GetOwner());
	check(OwningCharacter);

	MovementComponent = OwningCharacter->GetCharacterMovement();
	check(MovementComponent);

	// Needed so we can "stick" character to wall
	MovementComponent->SetPlaneConstraintEnabled(true);

	// These values are changed when wall running, so we save them to change back when wall run ends
	DefaultGravityScale = MovementComponent->GravityScale;
	DefaultAirControl = MovementComponent->AirControl;

	OwningCharacter->LandedDelegate.AddDynamic(this, &UWallRunComponent::OnLanded);

	OwningCharacter->OnActorHit.AddDynamic(this, &UWallRunComponent::OnCharacterHit);
}


// Called every frame
void UWallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ClampHorizontalVelocity();

	if (bIsWallRunning) WallRunUpdate();

	CharacterRotationUpdate(DeltaTime);
}


void UWallRunComponent::Jump()
{
	if (UseAJump())
	{
		FVector LaunchVelocity = CalculateLaunchVelocity();
		// Override Z component of character's velocity
		OwningCharacter->LaunchCharacter(LaunchVelocity, false, true);
		if (bIsWallRunning) EndWallRun(EWallRunEndReason::JumpedOffWall);
	}
}

bool UWallRunComponent::UseAJump()
{
	if (bIsWallRunning)
	{
		// If we are wall running, jumping does not use up a jump
		return true;
	}
	else
	{
		if (JumpsLeft > 0)
		{
			JumpsLeft--;
			return true;
		}
	}

	return false;
}

void UWallRunComponent::SetJumps(int NumJumps)
{
	JumpsLeft = FMath::Clamp(NumJumps, 0, MaxJumps);
}

void UWallRunComponent::SetIsWallRunning(bool NewIsWallRunning)
{
	bIsWallRunning = NewIsWallRunning;
	OnWallRunStatusChanged.Broadcast(bIsWallRunning);
}

void UWallRunComponent::OnLanded(const FHitResult& Hit)
{
	SetJumps(MaxJumps);
}

FVector UWallRunComponent::CalculateLaunchVelocity()
{
	check(OwningCharacter)
	check(MovementComponent)

	FVector LaunchVelocity;

	if (bIsWallRunning)
	{
		// If on wall, jump away from wall
		switch (WallSide)
		{
		case EWallSide::Left:
			LaunchVelocity = FVector::CrossProduct(WallRunDirection, FVector(0.f, 0.f, -1.f));
			break;
		case EWallSide::Right:
			LaunchVelocity = FVector::CrossProduct(WallRunDirection, FVector(0.f, 0.f, 1.f));
			break;
		}
	}
	else if (MovementComponent->IsFalling())
	{
		// If in the air, launch in direction of player input
		LaunchVelocity = OwningCharacter->GetActorRightVector() * OwningCharacter->GetInputAxisValue(InputAxisRight)
			+ OwningCharacter->GetActorForwardVector() * OwningCharacter->GetInputAxisValue(InputAxisForward);
	}

	// No matter what, we want to go up
	LaunchVelocity += FVector(0.f, 0.f, 1.f);
	LaunchVelocity *= MovementComponent->JumpZVelocity;

	return LaunchVelocity;
}
FVector2D UWallRunComponent::GetHorizontalVelocity()
{
	check(MovementComponent)
	return FVector2D(MovementComponent->Velocity);
}

void UWallRunComponent::SetHorizontalVelocity(FVector2D NewVelocity)
{
	check(MovementComponent)
	MovementComponent->Velocity.X = NewVelocity.X;
	MovementComponent->Velocity.Y = NewVelocity.Y;
}

void UWallRunComponent::ClampHorizontalVelocity()
{
	check(MovementComponent)
	if (!MovementComponent->IsFalling()) return; // Only continue if character is in the air

	FVector2D HorizontalVelocity = GetHorizontalVelocity();
	float SpeedRatio = HorizontalVelocity.Size() / MovementComponent->GetMaxSpeed();

	if (SpeedRatio > 1)
	{
		SetHorizontalVelocity(HorizontalVelocity / SpeedRatio);
	}
}

bool UWallRunComponent::CanWallRunOnSurface(FVector SurfaceNormal)
{
	check(MovementComponent)
	// Cannot wall run on ceilings or surfaces that we can walk on
	return SurfaceNormal.Z > -0.05f
	&& SurfaceNormal.Z < MovementComponent->GetWalkableFloorZ();
}

void UWallRunComponent::OnCharacterHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	check(MovementComponent)
	if (bIsWallRunning || !MovementComponent->IsFalling()) return; // Make sure we are not grounded or already wall running

	// Check to see if we hit something that we can wall run on
	if (CanWallRunOnSurface(Hit.ImpactNormal))
	{
		WallSide = FindWallSide(Hit.ImpactNormal);
		if (WallRunKeysAreDown())
		{
			WallRunDirection = FindRunDirection(Hit.ImpactNormal, WallSide);
			BeginWallRun();
		}
	}
}

EWallSide UWallRunComponent::FindWallSide(FVector SurfaceNormal)
{
	check(OwningCharacter)
	float DotProductResult = FVector2D::DotProduct(FVector2D(SurfaceNormal), FVector2D(OwningCharacter->GetActorRightVector()));

	// If the character's right vector is in the same direction as the wall's surface normal, the wall is on the left.
	if (DotProductResult > 0) return EWallSide::Left;

	// Otherwise, the wall is on the right.
	return EWallSide::Right;
}

FVector UWallRunComponent::FindRunDirection(FVector SurfaceNormal, EWallSide Side)
{
	FVector RunDirection;
	switch (Side)
	{
	case EWallSide::Left:
		RunDirection = FVector::CrossProduct(SurfaceNormal, FVector(0.f, 0.f, 1.f));
		break;
	case EWallSide::Right:
		RunDirection = FVector::CrossProduct(SurfaceNormal, FVector(0.f, 0.f, -1.f));
		break;
	}
	return RunDirection;
}

bool UWallRunComponent::WallRunKeysAreDown()
{
	// Must be running forward to wall run
	if (OwningCharacter->GetInputAxisValue(InputAxisForward) < 0.1) return false;

	switch (WallSide)
	{
	case EWallSide::Left:
		return OwningCharacter->GetInputAxisValue(InputAxisRight) < -0.1f;
		break;
	case EWallSide::Right:
		return OwningCharacter->GetInputAxisValue(InputAxisRight) > 0.1f;
		break;
	}

	return false;
}

void UWallRunComponent::BeginWallRun()
{
	check(MovementComponent)
	// Restrict movement on the Z axis 
	MovementComponent->SetPlaneConstraintNormal({ 0.f, 0.f, 1.f });
	// Restrict rotation while wall running
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->GravityScale = WallRunGravityScale;
	MovementComponent->AirControl = WallRunAirControl;
	SetIsWallRunning(true);
}

void UWallRunComponent::WallRunUpdate()
{
	// If required keys are not held down, fall off wall
	if (!WallRunKeysAreDown()) EndWallRun(EWallRunEndReason::FellOffWall);

	FHitResult HitResult;
	bool bHitWall = LineTraceToWall(HitResult);

	// If line trace returns false, fall off wall
	if (!bHitWall) EndWallRun(EWallRunEndReason::FellOffWall);
	// If wall side has changed for some reason, fall off wall
	if (WallSide != FindWallSide(HitResult.ImpactNormal)) EndWallRun(EWallRunEndReason::FellOffWall);

	WallRunDirection = FindRunDirection(HitResult.ImpactNormal, WallSide);

	MovementComponent->Velocity.X = MovementComponent->GetMaxSpeed() * WallRunDirection.X;
	MovementComponent->Velocity.Y = MovementComponent->GetMaxSpeed() * WallRunDirection.Y;
}

void UWallRunComponent::CharacterRotationUpdate(float DeltaTime)
{
	check(OwningCharacter)
	// If we aren't wall running and the character roll is already zero, we don't need to do anything
	if (!bIsWallRunning && CurrentCharacterRoll == 0.f) return;

	FRotator CurrentRotation = OwningCharacter->GetActorRotation();

	if (bIsWallRunning)
	{
		// If we are wall running, we want to be rotated parallel to the wall
		CurrentRotation.Pitch = WallRunDirection.Rotation().Pitch;
		CurrentRotation.Yaw = WallRunDirection.Rotation().Yaw;
	}

	// Get target character roll, based on whether we are wall running and which side of the wall we are on
	float TargetRoll = GetTargetCharacterRoll();
	FRotator TargetRotation = { CurrentRotation.Pitch, CurrentRotation.Yaw, TargetRoll };

	// Smoothly interpolate towards target character roll
	CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, WallRunLeanSpeed);

	OwningCharacter->SetActorRotation(CurrentRotation);

	// Save current character roll for early return
	CurrentCharacterRoll = CurrentRotation.Roll;
}

void UWallRunComponent::EndWallRun(EWallRunEndReason Reason)
{
	check(MovementComponent)
	if (Reason == FellOffWall) UE_LOG(LogTemp, Warning, TEXT("Fell off wall"));
	Reason == EWallRunEndReason::FellOffWall ? SetJumps(1) : SetJumps(MaxJumps - 1);
	MovementComponent->SetPlaneConstraintNormal({ 0.f, 0.f, 0.f });
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->GravityScale = DefaultGravityScale;
	MovementComponent->AirControl = DefaultAirControl;
	SetIsWallRunning(false);
}

bool UWallRunComponent::LineTraceToWall(FHitResult& HitResult)
{
	check(OwningCharacter)
	FVector LineTraceEnd;

	switch (WallSide)
	{
	case EWallSide::Left:
		LineTraceEnd = FVector::CrossProduct(WallRunDirection, FVector(0.f, 0.f, 1.f));
		break;
	case EWallSide::Right:
		LineTraceEnd = FVector::CrossProduct(WallRunDirection, FVector(0.f, 0.f, -1.f));
		break;
	}

	LineTraceEnd *= 200.f;

	LineTraceEnd += OwningCharacter->GetActorLocation();

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwningCharacter);

	return GetWorld()->LineTraceSingleByChannel(HitResult, OwningCharacter->GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

float UWallRunComponent::GetTargetCharacterRoll()
{
	if (bIsWallRunning)
	{
		if (WallSide == EWallSide::Left) return WallRunLean;
		if (WallSide == EWallSide::Right) return -WallRunLean;
	}
	// If we aren't wall running, we want to tilt the character back to normal
	return 0.f;
}