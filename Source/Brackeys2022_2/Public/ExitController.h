// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <PlayerCharacter.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitController.generated.h"

UCLASS()
class BRACKEYS2022_2_API AExitController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckWinCondition();

	APlayerCharacter* ChiefChicken;

	APlayerCharacter* Believer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Next Level")
	FName NextLevelName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyChickenArrived(APlayerCharacter* Player);

	void NotifyChickenGotOut(APlayerCharacter* Player);
};