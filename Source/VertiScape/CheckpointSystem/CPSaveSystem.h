// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPSaveSystem.generated.h"

UCLASS()
class VERTISCAPE_API ACPSaveSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPSaveSystem();

	void SaveGame(class UCPSaveGame* SaveGameInstance, bool bSaveAllSavables);

	void LoadSavedGame();

	void DeleteSavedGame();

	void Initialize(class ISaveSystemInterface* NewSaveInterface);

protected:
	UPROPERTY(EditAnywhere, Category = "Save Data")
	FString SaveSlotName;

	// Finds all actors that implement SavableInterface, and calls their SaveData function
	void SaveAllSavables(class UCPSaveGame* SaveGameInstance);

	// Finds all actors that implement SavableInterface, and calls their LoadData function
	void LoadAllSavables(UCPSaveGame* LoadedGame);

	UFUNCTION()
	void LoadSaveCallback(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	// Finds all checkpoints and initializes them - called after saved data is loaded
	void InitializeCheckpoints();

private:
	class ISaveSystemInterface* SaveInterface;
};