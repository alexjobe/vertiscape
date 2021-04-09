// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"

#include "VertiScape/CheckpointSystem/SavableInterface.h"

#include "Coin.generated.h"

UCLASS()
class VERTISCAPE_API ACoin : public APickupBase, public ISavableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

	//////////////////////////////////////////////////////////////////////////
	// SavableInterface
	virtual FSavableData SaveData() override;
	virtual void LoadData(FSavableData DataToLoad) override;
	virtual FString GetLastCheckpointName() override;
	virtual void SetLastCheckpointName(FString NewCheckpointName) override;

protected:
	FString LastCheckpointName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
