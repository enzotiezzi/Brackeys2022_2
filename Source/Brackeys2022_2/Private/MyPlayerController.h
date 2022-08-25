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

	virtual void OnPossess(APawn* NewPawn) override;

	UFUNCTION()
	void PauseGame();

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void CallGameOver();

	UFUNCTION()
	void RetryLevel();

protected:
	void MoveForward(float AxisValue, APawn* PawnToMove);

	void MoveSides(float AxisValue, APawn* PawnToMove);

	void MoveForwardLeftAnalog(float AxisValue);

	void MoveSidesLeftAnalog(float AxisValue);

	void MoveForwardRightAnalog(float AxisValue);

	void MoveSidesRightAnalog(float AxisValue);

	/*
	*
	* UI
	* 
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetRef;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* PauseWidget;

	void SetupPauseMenuWidget();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetRef;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* GameOverWidget;

	void SetupGameOverWidget();

	/*
	*
	* SOUND
	*
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;	void MakeNoiseCurrentPawn();

	void BelieverMakeNoise();

	void MakeNoise(APawn* PawnToMakeNoise);
};