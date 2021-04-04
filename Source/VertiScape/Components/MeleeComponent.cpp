// Made by Alex Jobe


#include "MeleeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
	bIsAttacking = false;
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
}

void UMeleeComponent::BeginAttack()
{
	if (bIsAttacking) return;
	check(MyOwner)

	bIsAttacking = true;
	OnBeginAttack.Broadcast();

	// Set timer to create hit zone
	MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_TimeToHit, this, &UMeleeComponent::CreateHitSphere, TimeToHit);
	MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_TimeToReset, this, &UMeleeComponent::ResetAttack, TimeToReset);
}

void UMeleeComponent::CreateHitSphere()
{
	TArray <AActor*> HitActors;
	bool bHitSomething = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetComponentLocation(), HitSphereRadius, ObjectTypes, nullptr, ActorsToIgnore, HitActors);
	DrawDebugSphere(GetWorld(), GetComponentLocation(), HitSphereRadius, 12, FColor::Red, false, 2.f);

	if (bHitSomething)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit something!"));
	}
}

void UMeleeComponent::ResetAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack reset!"));
	bIsAttacking = false;
}