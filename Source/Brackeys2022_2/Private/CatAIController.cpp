// Fill out your copyright notice in the Description page of Project Settings.


#include "CatAIController.h"

#include <AIModule/Classes/Perception/AIPerceptionComponent.h>
#include <AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Hearing.h>
#include <Perception/AISenseConfig_Hearing.h>
#include <Perception/AISenseConfig_Sight.h>
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

		GetWorld()->GetTimerManager().ClearTimer(BackToPatrolTimer);
	}
}

void ACatAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACatAIController::OnTargetUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && Stimulus.GetAge() == 0.0 && Stimulus.WasSuccessfullySensed())
	{
		if (ACat* Cat = Cast<ACat>(GetPawn()))
		{
			Cat->GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;

			Cat->NotifySense(FText::FromString("?"));
		}

		GetBlackboardComponent()->SetValueAsBool("HasNoise", true);
		GetBlackboardComponent()->SetValueAsVector("NoiseLocation", Stimulus.StimulusLocation);
	
		if (BackToPatrolTimer.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(BackToPatrolTimer);

		GetWorld()->GetTimerManager().SetTimer(BackToPatrolTimer, [this]() 
			{
				if (BackToPatrolTimer.IsValid())
				{
					GetWorld()->GetTimerManager().ClearTimer(BackToPatrolTimer);

					GetBlackboardComponent()->SetValueAsBool("HasNoise", false);
				}
			}, GetBackToPatrolInSeconds, false);
	}
	else
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				GetWorld()->GetTimerManager().ClearTimer(BackToPatrolTimer);

				GetBlackboardComponent()->SetValueAsBool("HasNoise", false);

				if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
				{
					if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
					{
						if (ACat* Cat = Cast<ACat>(GetPawn()))
						{
							Cat->GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;

							Cat->NotifySense(FText::FromString("!"));
						}

						Player->NotifyFound();

						if (bCanChaseEnemy)
						{
							CurrentPlayer = Player;

							GetBlackboardComponent()->SetValueAsObject("Target", CurrentPlayer);
						}
					}
				}
			}
			else
			{
				FActorPerceptionBlueprintInfo Info;

				AIPerceptionComponent->GetActorsPerception(CurrentPlayer, Info);

				if (Info.LastSensedStimuli.Num() > 0)
				{
					if (!Info.LastSensedStimuli[0].WasSuccessfullySensed())
					{
						if (!GetBlackboardComponent()->GetValueAsBool("HasClue"))
						{
							GetBlackboardComponent()->SetValueAsBool("HasClue", true);
							GetBlackboardComponent()->SetValueAsVector("ClueLocation", CurrentPlayer->GetActorLocation());
						}

						bCanChaseEnemy = false;

						if (CanChaseEnemyTimerHandle.IsValid())
							GetWorld()->GetTimerManager().ClearTimer(CanChaseEnemyTimerHandle);

						GetWorld()->GetTimerManager().SetTimer(CanChaseEnemyTimerHandle, [this]() 
							{
								bCanChaseEnemy = true;
							}, CooldownToChaseTargetAgain, false);

						CurrentPlayer = nullptr;

						GetBlackboardComponent()->SetValueAsObject("Target", CurrentPlayer);
					}
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

void ACatAIController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(BackToPatrolTimer);	
	GetWorld()->GetTimerManager().ClearTimer(CanChaseEnemyTimerHandle);
}