// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CheckpointSystem/CPInterface.h"

#include "VertiScapeGameMode.generated.h"

UCLASS(minimalapi)
class AVertiScapeGameMode : public AGameModeBase, public ICPInterface
{
	GENERATED_BODY()

public:
	AVertiScapeGameMode();

	void StartPlay() override;

	void SaveCheckpoint(ACheckpoint* NewCheckpoint) override;

	UFUNCTION(BlueprintCallable)
	void LoadCheckpoint();

	UFUNCTION()
	void LoadCheckpointCallback(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);
};



