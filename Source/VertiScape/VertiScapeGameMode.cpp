// Copyright Epic Games, Inc. All Rights Reserved.

#include "VertiScapeGameMode.h"
#include "VertiScapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CheckpointSystem/Checkpoint.h"
#include "CheckpointSystem/CPSaveGame.h"
#include "CheckpointSystem/SavableInterface.h"

AVertiScapeGameMode::AVertiScapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AVertiScapeGameMode::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ACheckpoint* CP = Cast<ACheckpoint>(FoundActor);
		if (CP) CP->SetCheckpointInterface(this);
	}

	LoadCheckpoint();
}

void AVertiScapeGameMode::SaveCheckpoint(ACheckpoint* NewCheckpoint)
{
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Saved!"));

	if (UCPSaveGame* SaveGameInstance = Cast<UCPSaveGame>(UGameplayStatics::CreateSaveGameObject(UCPSaveGame::StaticClass())))
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavableInterface::StaticClass(), FoundActors);

		for (AActor* FoundActor : FoundActors)
		{
			ISavableInterface* SavableActor = Cast<ISavableInterface>(FoundActor);
			if (SavableActor)
			{
				FSavableData SavableData = SavableActor->SaveData();
				SaveGameInstance->SavableDataMap.Add(
					UKismetSystemLibrary::GetObjectName(FoundActor), SavableData
				);
			}
		}

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, "Save1", 0);
	}
}

void AVertiScapeGameMode::LoadCheckpoint()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &AVertiScapeGameMode::LoadCheckpointCallback);
	UGameplayStatics::AsyncLoadGameFromSlot("Save1", 0, LoadedDelegate);
}

void AVertiScapeGameMode::LoadCheckpointCallback(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	if (UCPSaveGame* LoadedGame = Cast<UCPSaveGame>(LoadedGameData))
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavableInterface::StaticClass(), FoundActors);

		for (AActor* FoundActor : FoundActors)
		{
			ISavableInterface* SavableActor = Cast<ISavableInterface>(FoundActor);
			check(SavableActor)
			FString FoundActorName = UKismetSystemLibrary::GetObjectName(FoundActor);
			UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *FoundActorName);
			if (LoadedGame->SavableDataMap.Contains(FoundActorName))
			{
				FSavableData ActorsData = LoadedGame->SavableDataMap[FoundActorName];
				SavableActor->LoadData(ActorsData);
			}
		}
	}

	TArray<AActor*> AllCheckpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllCheckpoints);

	for (AActor* FoundCheckpoint : AllCheckpoints)
	{
		FoundCheckpoint->SetActorEnableCollision(true);
	}
}
