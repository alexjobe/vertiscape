// Made by Alex Jobe


#include "CPSaveSystem.h"
#include "Kismet/GameplayStatics.h"

#include "Checkpoint.h"
#include "CPSaveGame.h"
#include "SaveSystemInterface.h"

// Sets default values
ACPSaveSystem::ACPSaveSystem()
{
	PrimaryActorTick.bCanEverTick = false;
	SaveSlotName = "TestSave";
}

void ACPSaveSystem::Initialize(class ISaveSystemInterface* NewSaveInterface)
{
	this->SaveInterface = NewSaveInterface;
	LoadSavedCheckpoint();
}

void ACPSaveSystem::SaveCheckpoint(class UCPSaveGame* SaveGameInstance)
{
	if (SaveGameInstance)
	{
		SaveAllSavables(SaveGameInstance);
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint Saved!"));
	}
}

void ACPSaveSystem::SaveAllSavables(UCPSaveGame* SaveGameInstance)
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
}

void ACPSaveSystem::LoadSavedCheckpoint()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &ACPSaveSystem::LoadCheckpointCallback);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, LoadedDelegate);
}

void ACPSaveSystem::LoadCheckpointCallback(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	if (UCPSaveGame* LoadedGame = Cast<UCPSaveGame>(LoadedGameData))
	{
		LoadAllSavables(LoadedGame);
		SaveInterface->LoadCheckpoint(LoadedGame);
	}

	InitializeCheckpoints();
}

void ACPSaveSystem::LoadAllSavables(UCPSaveGame* LoadedGame)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavableInterface::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ISavableInterface* SavableActor = Cast<ISavableInterface>(FoundActor);
		if (SavableActor)
		{
			FString FoundActorName = UKismetSystemLibrary::GetObjectName(FoundActor);
			if (LoadedGame->SavableDataMap.Contains(FoundActorName))
			{
				FSavableData ActorsData = LoadedGame->SavableDataMap[FoundActorName];
				SavableActor->LoadData(ActorsData);
			}
		}
	}
}

void ACPSaveSystem::InitializeCheckpoints()
{
	TArray<AActor*> AllCheckpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), AllCheckpoints);

	for (AActor* FoundActor : AllCheckpoints)
	{
		ACheckpoint* Checkpoint = Cast<ACheckpoint>(FoundActor);
		if (Checkpoint)
		{
			if(SaveInterface) Checkpoint->SetSaveInterface(SaveInterface);

			if (Checkpoint->GetCheckpointStatus() == ECheckpointStatus::Inactive)
			{
				Checkpoint->DisableCheckpoint();
			}
			else
			{
				Checkpoint->EnableCheckpoint();
			}
		}
	}
}

void ACPSaveSystem::DeleteSavedCheckpoint()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	UE_LOG(LogTemp, Warning, TEXT("Save Deleted!"));
}
