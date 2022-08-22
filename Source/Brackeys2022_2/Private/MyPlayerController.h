// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <PlayerCharacter.h>
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	void MoveForward(float AxisValue);

	void MoveSides(float AxisValue);

	void MoveForwardLeftAnalog(float AxisValue);

	void MoveSidesLeftAnalog(float AxisValue);

	void MoveForwardRightAnalog(float AxisValue);

	void MoveSidesRightAnalog(float AxisValue);
};