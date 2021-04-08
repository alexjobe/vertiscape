// Made by Alex Jobe


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetAutoActivate(true);

	//Initialize the player's Health
	DefaultHealth = 100.f;
	CurrentHealth = DefaultHealth;
	bIsDead = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();
	check(MyOwner)

	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || Damage <= 0.f) return;
	SetCurrentHealth(CurrentHealth - Damage);
}

void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	if (MyOwner)
	{
		float OldHealth = CurrentHealth;
		CurrentHealth = FMath::Clamp(NewHealth, 0.f, DefaultHealth);
		OnHealthUpdate(OldHealth);
	}
}

void UHealthComponent::OnHealthUpdate(float OldHealth)
{
	float HealthChangeAmount = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(this, CurrentHealth, HealthChangeAmount, nullptr, nullptr, nullptr);
}
