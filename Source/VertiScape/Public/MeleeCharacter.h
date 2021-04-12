// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "VertiScape/CheckpointSystem/SavableInterface.h"

#include "MeleeCharacter.generated.h"

UCLASS()
class VERTISCAPE_API AMeleeCharacter : public ACharacter, public ISavableInterface
{
	GENERATED_BODY()

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Sets default values for this character's properties
	AMeleeCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns MeleeComp subobject **/
	FORCEINLINE class UMeleeComponent* GetMeleeComp() const { return MeleeComp; }

	/** Returns HealthComp subobject **/
	FORCEINLINE class UHealthComponent* GetHealthComp() const { return HealthComp; }
	
	//////////////////////////////////////////////////////////////////////////
	// SavableInterface
	virtual FSavableData SaveData() override;

	virtual void LoadData(FSavableData DataToLoad) override;

protected:
	// Character is dead
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsDead;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComponent, float Health, float HealthChangeAmount, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	virtual void DisableCharacter();

	/** Calls BeginAttack on MeleeComponent */
	UFUNCTION(BlueprintCallable)
	void BeginMeleeAttack();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Melee component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* MeleeComp;

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"));
	class UHealthComponent* HealthComp;
};