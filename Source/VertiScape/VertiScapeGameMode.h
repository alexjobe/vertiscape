// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CheckpointSystem/SaveSystemInterface.h"

#include "VertiScapeGameMode.generated.h"

UCLASS(minimalapi)
class AVertiScapeGameMode : public AGameModeBase, public ISaveSystemInterface
{
	GENERATED_BODY()

public:
	AVertiScapeGameMode();

	void StartPlay() override;

	virtual void SaveCheckpoint() override;

	virtual void LoadCheckpoint(class UCPSaveGame* SaveGameInstance) override;

	virtual void ResetLevel() override;

	UFUNCTION(BlueprintCallable)
	void DeleteSavedGame();

	UFUNCTION(BlueprintCallable)
	void AddCoin();

	void StartGameTimer();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumCollectedCoins() const { return NumCollectedCoins; }

protected:
	// The number of coins that must be collected
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 NumCoinsToFind;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timers")
	float BestGameTime;

	UPROPERTY(BlueprintReadOnly, Category = "Timers")
	float CurrentGameTime;

	FTimerHandle TimerHandle_GameTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timers")
	float TimeToRestart;

	FTimerHandle TimerHandle_TimeToRestart;

	void EndGame();

	UFUNCTION()
	void GameTimeUpdate();

private:
	class ACPSaveSystem* SaveSystem;

	// How many coins have been collected so far
	int32 NumCollectedCoins;
};