// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTGetNextPathPoint.h"

#include <AIModule/Classes/AIController.h>
#include <Cat.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <CatPathPoint.h>

UBTTGetNextPathPoint::UBTTGetNextPathPoint()
{

}

EBTNodeResult::Type UBTTGetNextPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACat* Cat = Cast<ACat>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Cat->CurrentPathPoint++;

		FVector NextPoint = Cat->Path[Cat->CurrentPathPoint % Cat->Path.Num()]->GetActorLocation();

		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PathPoint", NextPoint);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}