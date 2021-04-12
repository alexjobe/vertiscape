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
};
