// Made by Alex Jobe


#include "Checkpoint.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent({ 64.f, 64.f, 64.f });
	RootComponent = TriggerBox;
}

void ACheckpoint::SetCheckpointInterface(ICPInterface* NewCheckpointInterface)
{
	this->CheckpointInterface = NewCheckpointInterface;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnTriggerOverlap);
}

void ACheckpoint::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CheckpointInterface)
	{
		CheckpointInterface->SaveCheckpoint();
	}
}

