// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Custom OnHealthChanged event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthChangeAmount, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERTISCAPE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetDefaultHealth() const { return DefaultHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	// Clamps the value between 0 and DefaultHealth and calls OnHealthUpdate
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float NewHealth);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

protected:
	// The owner's default health. This is the value their health starts at when spawned.
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float DefaultHealth;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Function to bind to owning actor's OnTakeAnyDamage delegate
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// Response to health being updated
	void OnHealthUpdate(float OldHealth);

private:
	AActor* MyOwner;

	bool bIsDead;

	float CurrentHealth;
};
