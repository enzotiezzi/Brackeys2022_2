// Fill out your copyright notice in the Description page of Project Settings.


#include "CatAIController.h"

ACatAIController::ACatAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACatAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree) 
	{
		RunBehaviorTree(BehaviorTree);
	}
}