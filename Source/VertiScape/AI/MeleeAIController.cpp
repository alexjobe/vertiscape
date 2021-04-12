// Made by Alex Jobe


#include "MeleeAIController.h"

#include "VertiScape/VertiScape.h"

AMeleeAIController::AMeleeAIController()
{
	SetGenericTeamId(FGenericTeamId(TEAM_ID_AI));
}

ETeamAttitude::Type AMeleeAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}