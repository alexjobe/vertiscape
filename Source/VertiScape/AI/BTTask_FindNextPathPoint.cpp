// Made by Alex Jobe


#include "BTTask_FindNextPathPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "VertiScape/Components/PatrolPathComponent.h"

UBTTask_FindNextPathPoint::UBTTask_FindNextPathPoint()
{
	NodeName = TEXT("Find Next Path Point");
}

EBTNodeResult::Type UBTTask_FindNextPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Pawn && Blackboard)
	{
		UPatrolPathComponent* PatrolComp = Cast<UPatrolPathComponent>(Pawn->GetComponentByClass(UPatrolPathComponent::StaticClass()));
		if (PatrolComp)
		{
			if (PatrolComp->GetNumPathPoints() > 0)
			{
				int32 PathIndex = Blackboard->GetValueAsInt(PathIndexKey.SelectedKeyName);
				FVector PathPoint = PatrolComp->GetPathPoint(PathIndex);
				Blackboard->SetValueAsVector(PathVectorKey.SelectedKeyName, PathPoint);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
