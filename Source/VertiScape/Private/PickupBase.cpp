// Made by Alex Jobe


#include "PickupBase.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupBase::APickupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent({ 32.f, 32.f, 32.f });
	RootComponent = TriggerBox;

	bIsActive = true;

}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	bIsActive = true;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnTriggerOverlap);
}

void APickupBase::DisablePickup()
{
	bIsActive = false;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
}

void APickupBase::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only players can pick up
	if (!Cast<APawn>(OtherActor)->IsPlayerControlled()) return;

	DisablePickup();
}