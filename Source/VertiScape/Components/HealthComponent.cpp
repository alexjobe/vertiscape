// Made by Alex Jobe


#include "HealthComponent.h"
#include "GameFramework/Character.h"

#include "KnockBackDamageType.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetAutoActivate(true);

	//Initialize the player's Health
	DefaultHealth = 100.f;
	CurrentHealth = DefaultHealth;
	KnockBackResistance = 0.f;
	bIsDead = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	bIsDead = false;
	CurrentHealth = DefaultHealth;

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

	if (CurrentHealth > 0)
	{
		ApplyKnockBack(DamageType, DamageCauser);
	}
}

void UHealthComponent::ApplyKnockBack(const UDamageType* DamageType, AActor* DamageCauser)
{
	const UKnockBackDamageType* KnockBackDamageType = Cast<UKnockBackDamageType>(DamageType);
	if (KnockBackDamageType)
	{
		FVector LaunchVelocity = MyOwner->GetActorLocation() - DamageCauser->GetActorLocation();
		LaunchVelocity = { LaunchVelocity.X, LaunchVelocity.Y, 0.f };
		LaunchVelocity.Normalize();
		KnockBackResistance = FMath::Clamp(KnockBackResistance, 0.f, 1.f);
		float KnockBackForce = KnockBackDamageType->KnockBackForce * (1 - KnockBackResistance);
		LaunchVelocity *= KnockBackForce;

		ACharacter* MyOwningCharacter = Cast<ACharacter>(MyOwner);
		if (MyOwningCharacter)
		{
			MyOwningCharacter->LaunchCharacter(LaunchVelocity, true, false);
		}
	}
}

void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	if (MyOwner)
	{
		float OldHealth = CurrentHealth;
		CurrentHealth = FMath::Clamp(NewHealth, 0.f, DefaultHealth);
		if(CurrentHealth <= 0) bIsDead = true;
		OnHealthUpdate(OldHealth);
	}
}

void UHealthComponent::OnHealthUpdate(float OldHealth)
{
	float HealthChangeAmount = CurrentHealth - OldHealth;
	OnHealthChanged.Broadcast(this, CurrentHealth, HealthChangeAmount, nullptr, nullptr, nullptr);
}
