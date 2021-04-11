// Copyright Epic Games, Inc. All Rights Reserved.

#include "VertiScapeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "CheckpointSystem/CPSaveGame.h"
#include "CheckpointSystem/CPSaveSystem.h"
#include "VertiScapeSaveGame.h"

AVertiScapeGameMode::AVertiScapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	NumCollectedCoins = 0;
}

void AVertiScapeGameMode::StartPlay()
{
	Super::StartPlay();
	SaveSystem = Cast<ACPSaveSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPSaveSystem::StaticClass()));
	if(SaveSystem) SaveSystem->Initialize(this);
}

void AVertiScapeGameMode::SaveCheckpoint()
{
	if(!SaveSystem) return;

	UVertiScapeSaveGame* SaveGameInstance = Cast<UVertiScapeSaveGame>(UGameplayStatics::CreateSaveGameObject(UVertiScapeSaveGame::StaticClass()));
	if (SaveGameInstance) 
	{
		SaveGameInstance->NumCollectedCoins = NumCollectedCoins;
		SaveSystem->SaveCheckpoint(SaveGameInstance);
	}
}

void AVertiScapeGameMode::LoadCheckpoint(class UCPSaveGame* SaveGameInstance)
{
	UVertiScapeSaveGame* VertiScapeSaveGame = Cast<UVertiScapeSaveGame>(SaveGameInstance);
	if (VertiScapeSaveGame)
	{
		NumCollectedCoins = VertiScapeSaveGame->NumCollectedCoins;
	}
}

void AVertiScapeGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AVertiScapeGameMode::DeleteSavedCheckpoint()
{
	if (SaveSystem)
	{
		SaveSystem->DeleteSavedCheckpoint();
	}
}

void AVertiScapeGameMode::AddCoin()
{
	NumCollectedCoins++;
}
