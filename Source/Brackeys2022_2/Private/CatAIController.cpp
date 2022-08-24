// Fill out your copyright notice in the Description page of Project Settings.


#include "CatAIController.h"

#include <AIModule/Classes/Perception/AIPerceptionComponent.h>
#include <AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISenseConfig_Hearing.h>

ACatAIController::ACatAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/Cat/AI/BT_Cat"));

	if (BehaviorTreeFinder.Succeeded())
		BehaviorTree = BehaviorTreeFinder.Object;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACatAIController::OnTargetUpdated);
}

void ACatAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree) 
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ACatAIController::OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		GEngine->AddOnScreenDebugMessage(rand(), 1, FColor::Yellow, "Noise");
}

void ACatAIController::SetHearingRange(float HearingRange)
{
	UAISenseConfig_Hearing* HearingConfig = Cast<UAISenseConfig_Hearing>(AIPerceptionComponent->GetSenseConfig(UAISense::GetSenseID<UAISense_Hearing>()));

	HearingConfig->HearingRange = HearingRange;

	AIPerceptionComponent->ConfigureSense(*HearingConfig);
}