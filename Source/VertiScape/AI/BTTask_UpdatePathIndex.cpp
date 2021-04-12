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

	bool InitSuccess = InitVariables(OwnerComp);
	if(!InitSuccess) return EBTNodeResult::Failed;

	if (bIsMovingForward || bIsLooping)
	{
		IncrementPathIndex();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		DecrementPathIndex();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

bool UBTTask_UpdatePathIndex::InitVariables(UBehaviorTreeComponent& OwnerComp)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn) return false;

	UPatrolPathComponent* PatrolComp = Cast<UPatrolPathComponent>(Pawn->GetComponentByClass(UPatrolPathComponent::StaticClass()));
	if (!PatrolComp) return false;

	Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	NumPathPoints = PatrolComp->GetNumPathPoints();
	if(NumPathPoints <= 0) return false;

	bIsLooping = PatrolComp->GetIsLooping();
	bIsMovingForward = Blackboard->GetValueAsBool(IsMovingForwardKey.SelectedKeyName);
	PathIndex = Blackboard->GetValueAsInt(PathIndexKey.SelectedKeyName);

	return true;
}

void UBTTask_UpdatePathIndex::IncrementPathIndex()
{
	int32 NewPathIndex = PathIndex + 1;
	if (NewPathIndex < NumPathPoints)
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
}

void UBTTask_UpdatePathIndex::DecrementPathIndex()
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
}
