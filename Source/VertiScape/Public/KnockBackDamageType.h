// Made by Alex Jobe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "KnockBackDamageType.generated.h"

/**
 * 
 */
UCLASS()
class VERTISCAPE_API UKnockBackDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UKnockBackDamageType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockBackForce;
};