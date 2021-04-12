// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "MeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API AMeleeAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMeleeAIController();

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
