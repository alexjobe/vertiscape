// Made by Alex Jobe


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "SaveSystemInterface.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent({ 64.f, 64.f, 64.f });
	RootComponent = TriggerBox;
	CheckpointStatus = ECheckpointStatus::Uninitialized;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnTriggerOverlap);
}

void ACheckpoint::EnableCheckpoint()
{
	CheckpointStatus = ECheckpointStatus::Active;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorHiddenInGame(false);
}

void ACheckpoint::DisableCheckpoint()
{
	CheckpointStatus = ECheckpointStatus::Inactive;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
}

void ACheckpoint::SetSaveInterface(class ISaveSystemInterface* NewSaveInterface)
{
	this->SaveInterface = NewSaveInterface;
}

FSavableData ACheckpoint::SaveData()
{
	FSavableData SavableData;
	CheckpointStatus == ECheckpointStatus::Active ? SavableData.bIsActive = true : SavableData.bIsActive = false;
	return SavableData;
}

void ACheckpoint::LoadData(FSavableData DataToLoad)
{
	DataToLoad.bIsActive == true ? CheckpointStatus = ECheckpointStatus::Active : CheckpointStatus = ECheckpointStatus::Inactive;
}

void ACheckpoint::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only players should activate checkpoints
	if (!Cast<APawn>(OtherActor)->IsPlayerControlled()) return;

	if (SaveInterface && CheckpointStatus == ECheckpointStatus::Active)
	{
		DisableCheckpoint();
		SaveInterface->SaveCheckpoint();
	}
}