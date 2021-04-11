// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SavableInterface.h"

#include "Checkpoint.generated.h"

UENUM()
enum ECheckpointStatus
{
	Active		    UMETA(DisplayName = "Active"),
	Inactive        UMETA(DisplayName = "Inactive"),
	Uninitialized   UMETA(DisplayName = "Uninitialized"),
};

UCLASS()
class VERTISCAPE_API ACheckpoint : public AActor, public ISavableInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACheckpoint();

	void SetSaveInterface(class ISaveSystemInterface* NewSaveInterface);

	void EnableCheckpoint();

	void DisableCheckpoint();

	FORCEINLINE ECheckpointStatus GetCheckpointStatus() const { return CheckpointStatus; }

	virtual FSavableData SaveData() override;

	virtual void LoadData(FSavableData DataToLoad) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ECheckpointStatus CheckpointStatus;

	class ISaveSystemInterface* SaveInterface;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
