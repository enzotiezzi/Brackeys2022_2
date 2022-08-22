// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>

AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::BeginPlay()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (PlayerCharacter->PlayerType == EPlayerType::Believer)
		{
			PlayerCharacter->SpringArmComponent->DestroyComponent();
			PlayerCharacter->CameraComponent->DestroyComponent();
		}
	}

	Super::BeginPlay();
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

void AMyPlayerController::MoveForward(float AxisValue)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		GetPawn()->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveSides(float AxisValue)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		GetPawn()->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveForwardLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveForward(AxisValue);
		}
	}
}

void AMyPlayerController::MoveSidesLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveSides(AxisValue);
		}
	}
}

void AMyPlayerController::MoveForwardRightAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)))
	{
		if (CurrentCharacter->PlayerType == EPlayerType::Believer)
		{
			MoveForward(AxisValue);
		}
	}
}

void AMyPlayerController::MoveSidesRightAnalog(float AxisValue)
{
	auto Player1 = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)))
	{
		if (CurrentCharacter->PlayerType == EPlayerType::Believer)
		{
			MoveSides(AxisValue);
		}
	}
}