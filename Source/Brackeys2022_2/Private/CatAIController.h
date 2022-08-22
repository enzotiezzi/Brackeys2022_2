// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComponent;
};
