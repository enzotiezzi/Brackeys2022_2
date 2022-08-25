// Fill out your copyright notice in the Description page of Project Settings.


#include "CatAIController.h"

#include <AIModule/Classes/Perception/AIPerceptionComponent.h>
#include <AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <Cat.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/AISense_Sight.h>
#include <PlayerCharacter.h>
#include <MyPlayerController.h>
#include <Kismet/GameplayStatics.h>

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

		GetBlackboardComponent()->SetValueAsBool("HasNoise", false);
	}
}

void ACatAIController::OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && Stimulus.GetAge() == 0.0)
	{
		if (ACat* Cat = Cast<ACat>(GetPawn()))
		{
			Cat->GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;

			Cat->NotifyNoise();
		}

		GetBlackboardComponent()->SetValueAsBool("HasNoise", true);
		GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
	
		if (ResetAgeTimer.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(ResetAgeTimer);

		GetWorld()->GetTimerManager().SetTimer(ResetAgeTimer, [this]() 
			{
				GetBlackboardComponent()->SetValueAsBool("HasNoise", false);

				if (ACat* Cat = Cast<ACat>(GetPawn()))
				{
					Cat->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
				}
			}, GetBackToPatrolInSeconds, false);
	}
	else
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
			{
				if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
				{
					GetWorld()->GetTimerManager().ClearTimer(ResetAgeTimer);

					PlayerController->CallGameOver();
				}
			}
		}
	}
}

void ACatAIController::SetHearingRange(float HearingRange)
{
	UAISenseConfig_Hearing* HearingConfig = Cast<UAISenseConfig_Hearing>(AIPerceptionComponent->GetSenseConfig(UAISense::GetSenseID<UAISense_Hearing>()));

	HearingConfig->HearingRange = HearingRange;

	AIPerceptionComponent->ConfigureSense(*HearingConfig);
}