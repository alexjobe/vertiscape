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

	void SaveCheckpoint();

	void LoadSavedCheckpoint();

	void DeleteSavedCheckpoint();

	void SetSaveInterface(class ISaveSystemInterface* NewSaveInterface);

protected:
	UPROPERTY(EditAnywhere, Category = "Save Data")
	FString SaveSlotName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Finds all checkpoints and sets their SaveSystem to this
	void InitializeCheckpoints();

	// Finds all actors that implement SavableInterface, and calls their SaveData function
	void SaveAllSavables(class UCPSaveGame* SaveGameInstance);

	// Finds all actors that implement SavableInterface, and calls their LoadData function
	void LoadAllSavables(UCPSaveGame* LoadedGame);

	UFUNCTION()
	void LoadCheckpointCallback(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	// Enables collision for all checkpoints - called after saved data is loaded
	void EnableCheckpoints();

private:
	class ISaveSystemInterface* SaveInterface;
};
