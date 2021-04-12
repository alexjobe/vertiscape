// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class VERTISCAPE_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bIsLooping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WaitTime;
};
