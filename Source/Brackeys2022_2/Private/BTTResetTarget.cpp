// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTResetTarget.h"

#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <CatAIController.h>

EBTNodeResult::Type UBTTResetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);

	return EBTNodeResult::Failed;
}