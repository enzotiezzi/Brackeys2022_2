// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTGetNextPathPoint.h"

#include <AIModule/Classes/AIController.h>
#include <Cat.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <CatPathPoint.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <CatAIController.h>

UBTTGetNextPathPoint::UBTTGetNextPathPoint()
{

}

EBTNodeResult::Type UBTTGetNextPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACat* Cat = Cast<ACat>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (ACatAIController* CatAIController = Cast<ACatAIController>(OwnerComp.GetAIOwner()))
		{
			Cat->GetCharacterMovement()->MaxWalkSpeed = CatAIController->PatrolSpeed;
		}

		int Mod = Cat->CurrentPathPoint % Cat->Path.Num();

		GEngine->AddOnScreenDebugMessage(rand(), 1, FColor::Red, FString::FromInt(Mod));

		if (Mod == 0)
			Cat->SetActorRotation(Cat->Path[0]->GetActorRotation());

		Cat->CurrentPathPoint++;

		FVector NextPoint = Cat->Path[Mod]->GetActorLocation();

		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PathPoint", NextPoint);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}