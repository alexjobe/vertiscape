// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "MeleeCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API APlayerCharacter : public AMeleeCharacter
{
	GENERATED_BODY()

	/** Wall running component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WallRunning, meta = (AllowPrivateAccess = "true"))
	class UWallRunComponent* WallRunComp;

public:
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UWallRunComponent* GetWallRunComp() const { return WallRunComp; }

protected:
	/** Calls Jump on WallRunComponent */
	void WallRunJump();
};
