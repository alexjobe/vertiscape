// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MeleeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginAttackSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VERTISCAPE_API UMeleeComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMeleeComponent();

	UFUNCTION(BlueprintCallable)
	void BeginAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timers")
	float TimeToHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timers")
	float TimeToReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "What to Hit")
	float HitSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "What to Hit")
	TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "What to Hit")
	TArray <AActor*> ActorsToIgnore;

	FTimerHandle TimerHandle_TimeToHit;

	FTimerHandle TimerHandle_TimeToReset;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBeginAttackSignature OnBeginAttack;

	UFUNCTION()
	void CreateHitSphere();

	UFUNCTION()
	void ResetAttack();

private:
	AActor* MyOwner;
};