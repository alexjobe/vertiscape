// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableInterface.generated.h"

USTRUCT()
struct FSavableData
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	bool bIsActive;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VERTISCAPE_API ISavableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FSavableData SaveData() = 0;
	virtual void LoadData(FSavableData DataToLoad) = 0;
};
