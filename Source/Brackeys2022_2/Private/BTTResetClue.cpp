// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTResetClue.h"

#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

EBTNodeResult::Type UBTTResetClue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("HasClue", false);

	return EBTNodeResult::Succeeded;
}