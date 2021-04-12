// Made by Alex Jobe


#include "VertiScapePlayerController.h"

#include "VertiScape/VertiScape.h"

AVertiScapePlayerController::AVertiScapePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(TEAM_ID_PLAYERS);
}

FGenericTeamId AVertiScapePlayerController::GetGenericTeamId() const
{
	return TeamId;
}
