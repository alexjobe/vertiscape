// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpdatePathIndex.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API UBTTask_UpdatePathIndex : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_UpdatePathIndex();

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PathIndexKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector IsMovingForwardKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Initialize the variables we need
	bool InitVariables(UBehaviorTreeComponent& OwnerComp);

	// Called when the pawn is moving forwards along the path
	void IncrementPathIndex();

	// Called when the pawn is moving backwards along the path
	void DecrementPathIndex();

private:
	class UBlackboardComponent* Blackboard;
	bool bIsLooping;
	bool bIsMovingForward;
	int32 PathIndex;
	int32 NumPathPoints;
};
