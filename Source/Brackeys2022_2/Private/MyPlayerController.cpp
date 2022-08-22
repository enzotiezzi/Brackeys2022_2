// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>
#include <Camera/CameraComponent.h>

AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::OnPossess(APawn* NewPawn)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(NewPawn))
	{
		if (PlayerCharacter->PlayerType == EPlayerType::Believer)
		{
			PlayerCharacter->SpringArmComponent->DestroyComponent();
			PlayerCharacter->CameraComponent->DestroyComponent();
		}
	}

	Super::OnPossess(NewPawn);
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("LeftAnalogForwardMovement", this, &AMyPlayerController::MoveForwardLeftAnalog);
	InputComponent->BindAxis("LeftAnalogSideMovement", this, &AMyPlayerController::MoveSidesLeftAnalog);
	InputComponent->BindAxis("RightAnalogForwardMovement", this, &AMyPlayerController::MoveForwardRightAnalog);
	InputComponent->BindAxis("RightAnalogSideMovement", this, &AMyPlayerController::MoveSidesRightAnalog);
}

void AMyPlayerController::MoveForward(float AxisValue, APawn* PawnToMove)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		PawnToMove->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveSides(float AxisValue, APawn* PawnToMove)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		PawnToMove->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveForwardLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MoveForward(AxisValue, CurrentCharacter);
	}
}

void AMyPlayerController::MoveSidesLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MoveSides(AxisValue, CurrentCharacter);
	}
}

void AMyPlayerController::MoveForwardRightAnalog(float AxisValue)
{
	APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn());

	if (APlayerCharacter* Player2Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()))
	{
		if (Player2Character->PlayerType == EPlayerType::Believer && CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveForward(AxisValue, Player2Character);
		}
	}
}

void AMyPlayerController::MoveSidesRightAnalog(float AxisValue)
{
	APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn());

	if (APlayerCharacter* Player2Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()))
	{
		if (Player2Character->PlayerType == EPlayerType::Believer && CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveSides(AxisValue, Player2Character);
		}
	}
}