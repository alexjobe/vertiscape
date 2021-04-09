// Copyright Epic Games, Inc. All Rights Reserved.

#include "VertiScapeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "CheckpointSystem/CPSaveSystem.h"

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
	SaveSystem = Cast<ACPSaveSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPSaveSystem::StaticClass()));
}

void AVertiScapeGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AVertiScapeGameMode::DeleteSavedCheckpoint()
{
	if (SaveSystem)
	{
		SaveSystem->DeleteSavedCheckpoint();
	}
}
