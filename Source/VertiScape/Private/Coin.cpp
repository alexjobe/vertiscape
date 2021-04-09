// Made by Alex Jobe


#include "Coin.h"

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

FString ACoin::GetLastCheckpointName()
{
	return LastCheckpointName;
}

void ACoin::SetLastCheckpointName(FString NewCheckpointName)
{
	LastCheckpointName = NewCheckpointName;
}

