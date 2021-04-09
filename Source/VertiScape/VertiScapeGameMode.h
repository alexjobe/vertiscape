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
	void DeleteSavedCheckpoint();

	UFUNCTION(BlueprintCallable)
	void AddCoin();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumCollectedCoins() const { return NumCollectedCoins; }

private:
	class ACPSaveSystem* SaveSystem;

	int32 NumCollectedCoins;
};