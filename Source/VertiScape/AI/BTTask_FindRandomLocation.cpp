// Made by Alex Jobe


#include "BTTask_FindRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn)
	{
		FVector PawnLocation = Pawn->GetActorLocation();
		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		if (NavSystem)
		{
			FVector NewLocation;
			NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), PawnLocation, NewLocation, 300.f, NULL, NULL);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, NewLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
