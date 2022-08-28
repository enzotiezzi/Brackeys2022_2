// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AIModule/Classes/Perception/AIPerceptionComponent.h>
#include <PlayerCharacter.h>
#include <AIModule/Classes/Perception/AIPerceptionTypes.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "CoreMinimal.h"
#include "AIController.h"
#include "CatAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACatAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus);

	void SetHearingRange(float HearingRange);

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle BackToPatrolTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float ChasingSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float PatrolSpeed = 200;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float GetBackToPatrolInSeconds = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* CurrentPlayer;
};
