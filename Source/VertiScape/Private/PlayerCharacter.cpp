// Made by Alex Jobe


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "VertiScape/Components/WallRunComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Wall Running
	WallRunComp = CreateDefaultSubobject<UWallRunComponent>(TEXT("WallRunning"));
	TimeToRespawn = 5.f;
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

void APlayerCharacter::DisableCharacter()
{
	Super::DisableCharacter();

	GetWorldTimerManager().SetTimer(TimerHandle_TimeToRespawn, this, &APlayerCharacter::RestartFromCheckpoint, TimeToRespawn);
}

void APlayerCharacter::RestartFromCheckpoint()
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode) GameMode->ResetLevel();
}