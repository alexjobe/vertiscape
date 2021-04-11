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
	NumCoinsToFind = 10;
	TimeToRestart = 3.f;
	CurrentGameTime = 0.f;
}

void AVertiScapeGameMode::StartPlay()
{
	Super::StartPlay();
	SaveSystem = Cast<ACPSaveSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPSaveSystem::StaticClass()));
	if(SaveSystem) SaveSystem->Initialize(this);
	StartGameTimer();
}

void AVertiScapeGameMode::SaveCheckpoint()
{
	if(!SaveSystem) return;

	UVertiScapeSaveGame* SaveGameInstance = Cast<UVertiScapeSaveGame>(UGameplayStatics::CreateSaveGameObject(UVertiScapeSaveGame::StaticClass()));
	if (SaveGameInstance) 
	{
		SaveGameInstance->NumCollectedCoins = NumCollectedCoins;
		SaveGameInstance->CurrentGameTime = CurrentGameTime;
		SaveSystem->SaveGame(SaveGameInstance, true);
	}
}

void AVertiScapeGameMode::LoadCheckpoint(class UCPSaveGame* SaveGameInstance)
{
	UVertiScapeSaveGame* VertiScapeSaveGame = Cast<UVertiScapeSaveGame>(SaveGameInstance);
	if (VertiScapeSaveGame)
	{
		NumCollectedCoins = VertiScapeSaveGame->NumCollectedCoins;
		CurrentGameTime = VertiScapeSaveGame->CurrentGameTime;
	}
}

void AVertiScapeGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AVertiScapeGameMode::DeleteSavedGame()
{
	if (SaveSystem) SaveSystem->DeleteSavedGame();
}

void AVertiScapeGameMode::AddCoin()
{
	NumCollectedCoins++;
	if(NumCollectedCoins >= NumCoinsToFind) EndGame();
}

void AVertiScapeGameMode::StartGameTimer()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GameTime, this, &AVertiScapeGameMode::GameTimeUpdate, 0.1f, true);
}

void AVertiScapeGameMode::GameTimeUpdate()
{
	CurrentGameTime += 0.1f;
}

void AVertiScapeGameMode::EndGame()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_GameTime);

	if (!SaveSystem) return;

	UVertiScapeSaveGame* SaveGameInstance = Cast<UVertiScapeSaveGame>(UGameplayStatics::CreateSaveGameObject(UVertiScapeSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		SaveGameInstance->NumCollectedCoins = 0;
		CurrentGameTime = 0.f;
		SaveSystem->SaveGame(SaveGameInstance, false);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_TimeToRestart, this, &AVertiScapeGameMode::ResetLevel, TimeToRestart);
}