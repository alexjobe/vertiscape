// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API UBTTask_FindNextPathPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindNextPathPoint();

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PathIndexKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PathVectorKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
