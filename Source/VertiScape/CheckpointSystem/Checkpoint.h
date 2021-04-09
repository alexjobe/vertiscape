// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"


UCLASS()
class VERTISCAPE_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	void SetSaveInterface(class ISaveSystemInterface* NewSaveInterface);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ISaveSystemInterface* SaveInterface;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};