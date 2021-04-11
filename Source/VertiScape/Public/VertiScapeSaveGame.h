// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "VertiScape/CheckpointSystem/CPSaveGame.h"
#include "VertiScapeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API UVertiScapeSaveGame : public UCPSaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	uint32 NumCollectedCoins;

	UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	float CurrentGameTime;

	UPROPERTY(VisibleAnywhere, Category = "High Score")
	float BestGameTime;

	UVertiScapeSaveGame();
};
