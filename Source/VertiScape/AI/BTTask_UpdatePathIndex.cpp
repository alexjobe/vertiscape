// Made by Alex Jobe


#include "BTTask_UpdatePathIndex.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PatrolPath.h"
#include "VertiScape/Components/PatrolPathComponent.h"

UBTTask_UpdatePathIndex::UBTTask_UpdatePathIndex()
{
	NodeName = TEXT("Update Patrol Path Index");
}

EBTNodeResult::Type UBTTask_UpdatePathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!Pawn) return EBTNodeResult::Failed;

	UPatrolPathComponent* PatrolComp = Cast<UPatrolPathComponent>(Pawn->GetComponentByClass(UPatrolPathComponent::StaticClass()));
	if(!PatrolComp) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	bool bIsLooping = PatrolComp->GetIsLooping();
	bool bIsMovingForward;
	int32 PathIndex;

	bIsMovingForward = Blackboard->GetValueAsBool(IsMovingForwardKey.SelectedKeyName);
	PathIndex = Blackboard->GetValueAsInt(PathIndexKey.SelectedKeyName);

	if (bIsMovingForward || bIsLooping)
	{
		int32 NewPathIndex = PathIndex + 1;
		if (NewPathIndex < PatrolComp->GetNumPathPoints())
		{
			Blackboard->SetValueAsInt(PathIndexKey.SelectedKeyName, NewPathIndex);
		}
		else if (bIsLooping)
		{
			Blackboard->SetValueAsInt(PathIndexKey.SelectedKeyName, 0);
		}
		else
		{
			Blackboard->SetValueAsBool(IsMovingForwardKey.SelectedKeyName, false);
		}

		return EBTNodeResult::Succeeded;
	}
	else
	{
		int32 NewPathIndex = PathIndex - 1;
		if (NewPathIndex >= 0)
		{
			Blackboard->SetValueAsInt(PathIndexKey.SelectedKeyName, NewPathIndex);
		}
		else
		{
			Blackboard->SetValueAsBool(IsMovingForwardKey.SelectedKeyName, true);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
