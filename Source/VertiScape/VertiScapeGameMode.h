// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VertiScapeGameMode.generated.h"

UCLASS(minimalapi)
class AVertiScapeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVertiScapeGameMode();

	void StartPlay() override;
};