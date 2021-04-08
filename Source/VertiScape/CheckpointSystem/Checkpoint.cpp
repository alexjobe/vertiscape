// Made by Alex Jobe


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "CPSaveSystem.h"
#include "SavableInterface.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent({ 64.f, 64.f, 64.f });
	RootComponent = TriggerBox;
}

void ACheckpoint::SetSaveSystem(class ACPSaveSystem* NewSaveSystem)
{
	this->SaveSystem = NewSaveSystem;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	// Checkpoints are disabled until data is finished loading
	SetActorEnableCollision(false);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnTriggerOverlap);
}

void ACheckpoint::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only players should activate checkpoints
	if (!Cast<APawn>(OtherActor)->IsPlayerControlled()) return;

	if (ISavableInterface* SavableActor = Cast<ISavableInterface>(OtherActor))
	{
		FString ActorLastCheckpoint = SavableActor->GetLastCheckpointName();
		// Make sure this checkpoint isn't the last checkpoint the player activated
		if (ActorLastCheckpoint == this->GetName()) return;
		// Update the player's last checkpoint to this one
		SavableActor->SetLastCheckpointName(this->GetName());

		if (SaveSystem)
		{
			SaveSystem->SaveCheckpoint(this);
		}
	}
}

