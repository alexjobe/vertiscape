// Copyright Epic Games, Inc. All Rights Reserved.

#include "VertiScapeGameMode.h"
#include "VertiScapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVertiScapeGameMode::AVertiScapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AVertiScapeGameMode::StartPlay()
{
	Super::StartPlay();
}