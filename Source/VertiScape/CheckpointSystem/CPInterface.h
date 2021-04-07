// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VERTISCAPE_API ICPInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SaveCheckpoint() = 0;
};
