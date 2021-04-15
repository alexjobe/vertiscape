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
	bIsJumping = false;

	WallRunGravityScale = 0.f;
	WallRunAirControl = 1.f;

	WallRunLean = 20.f;
	WallRunLeanSpeed = 10.f;
	TimeToFallOff = 0.5f;
	JumpsLeftAfterFalling = 1;
}


// Called when the game starts
void UWallRunComponent::BeginPlay()
{
	Super::BeginPlay();

	JumpsLeft = MaxJumps;

	MyOwner = Cast<ACharacter>(GetOwner());
	check(MyOwner);

	MovementComponent = MyOwner->GetCharacterMovement();
	check(MovementComponent);

	// Needed so we can "stick" character to wall
	MovementComponent->SetPlaneConstraintEnabled(true);

	// These values are changed when wall running, so we save them to change back when wall run ends
	DefaultGravityScale = MovementComponent->GravityScale;
	DefaultAirControl = MovementComponent->AirControl;

	MyOwner->LandedDelegate.AddDynamic(this, &UWallRunComponent::OnLanded);

	MyOwner->OnActorHit.AddDynamic(this, &UWallRunComponent::OnCharacterHit);
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
	if(!MyOwner) return;
	if (UseAJump())
	{
		bIsJumping = true;
		FVector LaunchVelocity = CalculateLaunchVelocity();
		// Override Z component of character's velocity
		MyOwner->LaunchCharacter(LaunchVelocity, false, true);
		if (bIsWallRunning) EndWallRun(EWallRunEndReason::JumpedOffWall);
	}
}

bool UWallRunComponent::UseAJump()
{

	if (JumpsLeft > 0)
	{
		JumpsLeft--;
		return true;
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
	bIsJumping = false;
}

FVector UWallRunComponent::CalculateLaunchVelocity()
{
	if(!MyOwner || !MovementComponent) return FVector::ZeroVector;

	FVector LaunchVelocity = FVector::ZeroVector;

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
		LaunchVelocity = MyOwner->GetActorRightVector() * MyOwner->GetInputAxisValue(InputAxisRight)
			+ MyOwner->GetActorForwardVector() * MyOwner->GetInputAxisValue(InputAxisForward);
	}

	// No matter what, we want to go up
	LaunchVelocity += FVector(0.f, 0.f, 1.f);
	LaunchVelocity *= MovementComponent->JumpZVelocity;

	return LaunchVelocity;
}
FVector2D UWallRunComponent::GetHorizontalVelocity()
{
	if (!MovementComponent) return FVector2D::ZeroVector;
	return FVector2D(MovementComponent->Velocity);

}

void UWallRunComponent::SetHorizontalVelocity(FVector2D NewVelocity)
{
	if(!MovementComponent) return;
	MovementComponent->Velocity.X = NewVelocity.X;
	MovementComponent->Velocity.Y = NewVelocity.Y;
}

void UWallRunComponent::ClampHorizontalVelocity()
{
	if (!bIsJumping) return; // Only continue if character is jumping
	if(!MovementComponent) return;

	FVector2D HorizontalVelocity = GetHorizontalVelocity();
	float SpeedRatio = HorizontalVelocity.Size() / MovementComponent->GetMaxSpeed();

	if (SpeedRatio > 1)
	{
		SetHorizontalVelocity(HorizontalVelocity / SpeedRatio);
	}
}

bool UWallRunComponent::CanWallRunOnSurface(FVector SurfaceNormal)
{
	if(!MovementComponent) return false;
	// Cannot wall run on ceilings or surfaces that we can walk on
	return SurfaceNormal.Z > -0.05f
	&& SurfaceNormal.Z < MovementComponent->GetWalkableFloorZ();
}

void UWallRunComponent::OnCharacterHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!MovementComponent) return;
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
	if (MyOwner)
	{
		float DotProductResult = FVector2D::DotProduct(FVector2D(SurfaceNormal), FVector2D(MyOwner->GetActorRightVector()));

		// If the character's right vector is in the same direction as the wall's surface normal, the wall is on the left.
		if (DotProductResult > 0) return EWallSide::Left;
	}

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
	if(!MyOwner) return false;
	// Must be running forward to wall run
	if (MyOwner->GetInputAxisValue(InputAxisForward) < 0.1) return false;

	switch (WallSide)
	{
	case EWallSide::Left:
		return MyOwner->GetInputAxisValue(InputAxisRight) < -0.1f;
		break;
	case EWallSide::Right:
		return MyOwner->GetInputAxisValue(InputAxisRight) > 0.1f;
		break;
	}

	return false;
}

void UWallRunComponent::BeginWallRun()
{
	if(!MovementComponent) return;
	// Restrict movement on the Z axis 
	MovementComponent->SetPlaneConstraintNormal({ 0.f, 0.f, 1.f });
	// Restrict rotation while wall running
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->GravityScale = WallRunGravityScale;
	MovementComponent->AirControl = WallRunAirControl;
	// Reset jumps
	SetJumps(MaxJumps);
	SetIsWallRunning(true);
}

void UWallRunComponent::WallRunUpdate()
{
	// If required keys are not held down, fall off wall
	FallOffIfKeysAreNotDown();

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

void UWallRunComponent::FallOffIfKeysAreNotDown()
{
	if(!MyOwner) return;
	// If required keys are not held down, we start a timer to determine if we should fall off. If the keys are still 
	// not held down when the timer completes, we fall off the wall. This gives the player time to correct the mistake
	if (!WallRunKeysAreDown() && !MyOwner->GetWorldTimerManager().IsTimerActive(TimerHandle_TimeToFallOff))
	{
		MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_TimeToFallOff, this, &UWallRunComponent::FallOffCallback, TimeToFallOff);
	}
}

void UWallRunComponent::FallOffCallback()
{
	if(!WallRunKeysAreDown()) EndWallRun(EWallRunEndReason::FellOffWall);
}

void UWallRunComponent::CharacterRotationUpdate(float DeltaTime)
{
	if(!MyOwner) return;
	// If we aren't wall running and the character roll is already zero, we don't need to do anything
	if (!bIsWallRunning && CurrentCharacterRoll == 0.f) return;

	FRotator CurrentRotation = MyOwner->GetActorRotation();

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

	MyOwner->SetActorRotation(CurrentRotation);

	// Save current character roll for early return
	CurrentCharacterRoll = CurrentRotation.Roll;
}

void UWallRunComponent::EndWallRun(EWallRunEndReason Reason)
{
	if(!MyOwner || !MovementComponent) return;

	MyOwner->GetWorldTimerManager().ClearTimer(TimerHandle_TimeToFallOff);
	if(Reason == EWallRunEndReason::FellOffWall) SetJumps(JumpsLeftAfterFalling);
	MovementComponent->SetPlaneConstraintNormal({ 0.f, 0.f, 0.f });
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->GravityScale = DefaultGravityScale;
	MovementComponent->AirControl = DefaultAirControl;
	SetIsWallRunning(false);
}

bool UWallRunComponent::LineTraceToWall(FHitResult& HitResult)
{
	if(!MyOwner) return false;
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

	LineTraceEnd += MyOwner->GetActorLocation();

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(MyOwner);

	return GetWorld()->LineTraceSingleByChannel(HitResult, MyOwner->GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
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