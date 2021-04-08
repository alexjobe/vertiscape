// Made by Alex Jobe


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"

#include "VertiScape/Components/WallRunComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Wall Running
	WallRunComp = CreateDefaultSubobject<UWallRunComponent>(TEXT("WallRunning"));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::WallRunJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void APlayerCharacter::WallRunJump()
{
	check(WallRunComp)
	WallRunComp->Jump();
}
