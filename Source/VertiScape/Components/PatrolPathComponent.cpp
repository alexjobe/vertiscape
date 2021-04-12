// Made by Alex Jobe


#include "PatrolPathComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "VertiScape/AI/PatrolPath.h"

// Sets default values for this component's properties
UPatrolPathComponent::UPatrolPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPatrolPathComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!PatrolPath) UE_LOG(LogTemp, Warning, TEXT("%s does not have a PatrolPath!"), *GetOwner()->GetName());
}

bool UPatrolPathComponent::GetIsLooping()
{
	if (PatrolPath) return PatrolPath->bIsLooping;
	return false;
}

float UPatrolPathComponent::GetWaitTime()
{
	if(PatrolPath) return PatrolPath->WaitTime;
	return 0.f;
}

int32 UPatrolPathComponent::GetNumPathPoints()
{
	if (PatrolPath) return PatrolPath->PathPoints.Num();
	return 0;
}

FVector UPatrolPathComponent::GetPathPoint(int32 Index)
{
	FVector ReturnVector;
	if (PatrolPath && Index < PatrolPath->PathPoints.Num())
	{
		ReturnVector = UKismetMathLibrary::TransformLocation(PatrolPath->GetActorTransform(), PatrolPath->PathPoints[Index]);
	}
	return ReturnVector;
}
