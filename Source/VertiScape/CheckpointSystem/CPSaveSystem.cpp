// Made by Alex Jobe


#include "CPSaveSystem.h"
#include "Kismet/GameplayStatics.h"

#include "CPSaveGame.h"
#include "Checkpoint.h"

// Sets default values
ACPSaveSystem::ACPSaveSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SaveSlotName = "TestSave";
}

// Called when the game starts or when spawned
void ACPSaveSystem::BeginPlay()
{
	Super::BeginPlay();

	InitializeCheckpoints();

	LoadSavedCheckpoint();
	
}
void ACPSaveSystem::InitializeCheckpoints()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		ACheckpoint* Checkpoint = Cast<ACheckpoint>(FoundActor);
		if (Checkpoint) Checkpoint->SetSaveSystem(this);
	}
}
void ACPSaveSystem::SaveCheckpoint(ACheckpoint* NewCheckpoint)
{
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Saved!"));

	if (UCPSaveGame* SaveGameInstance = Cast<UCPSaveGame>(UGameplayStatics::CreateSaveGameObject(UCPSaveGame::StaticClass())))
	{
		SaveAllSavables(SaveGameInstance);

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
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
	}

	EnableCheckpoints();
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

void ACPSaveSystem::EnableCheckpoints()
{
	TArray<AActor*> AllCheckpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllCheckpoints);

	for (AActor* FoundCheckpoint : AllCheckpoints)
	{
		FoundCheckpoint->SetActorEnableCollision(true);
	}
}
