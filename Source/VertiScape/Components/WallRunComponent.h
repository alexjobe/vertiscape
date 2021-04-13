// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallRunComponent.generated.h"

UENUM()
enum EWallSide
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right")
};

UENUM()
enum EWallRunEndReason
{
	FellOffWall	    UMETA(DisplayName = "FellOffWall"),
	JumpedOffWall	UMETA(DisplayName = "JumpedOffWall")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWallRunStatusChangedSignature, bool, IsWallRunning);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VERTISCAPE_API UWallRunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWallRunComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Jump();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	FName InputAxisRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	FName InputAxisForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Jumping)
	int MaxJumps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning)
	float WallRunGravityScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning)
	float WallRunAirControl;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning, meta = (ToolTip = "How much should the character lean away from the wall while wall running?"))
	float WallRunLean;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning, meta = (ToolTip = "How fast should the character lean away from the wall while wall running?"))
	float WallRunLeanSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning, meta = (ToolTip = "How long should the character keep wall running once wall run keys are no longer held down?"))
	float TimeToFallOff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WallRunning, meta = (ToolTip = "How many jumps should the character have left after falling off a wall?"))
	int32 JumpsLeftAfterFalling;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWallRunStatusChangedSignature OnWallRunStatusChanged;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ACharacter* MyOwner;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	bool bIsWallRunning;

	// How many jumps we currently have
	int JumpsLeft;

	bool bIsJumping;

	FVector WallRunDirection;

	TEnumAsByte<EWallSide> WallSide;

	float DefaultGravityScale;

	float DefaultAirControl;

	float CurrentCharacterRoll;

	FTimerHandle TimerHandle_TimeToFallOff;

	// If we can jump, decrements JumpsLeft and returns true
	bool UseAJump();

	void SetJumps(int NumJumps);

	// Sets bIsWallRunning and triggers OnWallRunStatusChanged event
	void SetIsWallRunning(bool NewIsWallRunning);

	// Used to determine jump velocity
	FVector CalculateLaunchVelocity();

	// Binds to the character's LandedDelegate, to be called when character stops falling
	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	// Returns X and Y components of character's velocity as a 2D vector
	FVector2D GetHorizontalVelocity();

	void SetHorizontalVelocity(FVector2D NewVelocity);

	// Since jumping adds horizontal velocity, we want to clamp velocity to a max speed so
	// the character doesn't keep speeding up with each consecutive jump
	void ClampHorizontalVelocity();

	// Returns true if we can wall run on a given surface, provided the surface normal
	bool CanWallRunOnSurface(FVector SurfaceNormal);

	// Called when we collide with something, so we can check if we hit a wall we can run on
	UFUNCTION()
	void OnCharacterHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	// Find out which side the wall is on, based on its surface normal
	EWallSide FindWallSide(FVector SurfaceNormal);

	FVector FindRunDirection(FVector SurfaceNormal, EWallSide Side);

	// Returns true if the required keys are held down to wall run
	bool WallRunKeysAreDown();

	void BeginWallRun();

	void WallRunUpdate();

	void FallOffIfKeysAreNotDown();

	UFUNCTION()
	void FallOffCallback();

	void EndWallRun(EWallRunEndReason Reason);

	bool LineTraceToWall(FHitResult& HitResult);

	// Tilt the character away from the wall while wall running
	void CharacterRotationUpdate(float DeltaTime);

	float GetTargetCharacterRoll();
};
