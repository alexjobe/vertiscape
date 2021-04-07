// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SavableInterface.h"

#include "CPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API UCPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Save Data")
	TMap<FString, FSavableData> SavableDataMap;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	
	UCPSaveGame();
};
