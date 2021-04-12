// Made by Alex Jobe


#include "MeleeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetAutoActivate(true);

	bIsAttacking = false;
	Damage = 10.f;
	HitSphereRadius = 80.f;
	TimeToHit = 0.2f;
	TimeToReset = 0.35f;
}

// Called when the game starts
void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = GetOwner();
	check(MyOwner)
	ActorsToIgnore.Add(MyOwner);
	bIsAttacking = false;
}

void UMeleeComponent::BeginAttack()
{
	if (bIsAttacking) return;
	if (!MyOwner) return;

	bIsAttacking = true;
	OnBeginAttack.Broadcast();

	// Set timer to create hit zone
	MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_TimeToHit, this, &UMeleeComponent::CreateHitSphere, TimeToHit);
	MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_TimeToReset, this, &UMeleeComponent::ResetAttack, TimeToReset);
}

void UMeleeComponent::CreateHitSphere()
{
	//DrawDebugSphere(GetWorld(), GetComponentLocation(), HitSphereRadius, 12, FColor::Red, false, 2.f);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetComponentLocation(), HitSphereRadius, DamageType, ActorsToIgnore, MyOwner, MyOwner->GetInstigatorController(), true);
}

void UMeleeComponent::ResetAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack reset!"));
	bIsAttacking = false;
}