// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "VertiScapePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API AVertiScapePlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AVertiScapePlayerController();

private:
	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};
