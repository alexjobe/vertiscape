// Made by Alex Jobe


#include "Coin.h"
#include "Kismet/GameplayStatics.h"

#include "VertiScape/VertiScapeGameMode.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
}

void ACoin::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnTriggerOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (!Cast<APawn>(OtherActor)->IsPlayerControlled()) return;
	AVertiScapeGameMode* GameMode = Cast<AVertiScapeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode) GameMode->AddCoin();
}

//////////////////////////////////////////////////////////////////////////
// Save System

FSavableData ACoin::SaveData()
{
	FSavableData SavableData;
	SavableData.bIsActive = bIsActive;
	return SavableData;
}

void ACoin::LoadData(FSavableData DataToLoad)
{
	if (!DataToLoad.bIsActive) DisablePickup();
}

