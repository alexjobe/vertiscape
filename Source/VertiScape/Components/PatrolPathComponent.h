// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolPathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERTISCAPE_API UPatrolPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolPathComponent();

	UFUNCTION(BlueprintCallable)
	bool GetIsLooping();

	UFUNCTION(BlueprintCallable)
	float GetWaitTime();

	UFUNCTION(BlueprintCallable)
	int32 GetNumPathPoints();

	UFUNCTION(BlueprintCallable)
	FVector GetPathPoint(int32 Index);

protected:
	UPROPERTY(EditInstanceOnly)
	class APatrolPath* PatrolPath;

		
};
