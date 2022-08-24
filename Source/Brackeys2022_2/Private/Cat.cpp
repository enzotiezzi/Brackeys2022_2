// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"
#include <CatAIController.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

// Sets default values
ACat::ACat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	
	if (ACatAIController* AIController = Cast<ACatAIController>(GetController()))
	{
		if (Path.Num() > 0)
		{
			CurrentPathPoint = 0;

			AIController->GetBlackboardComponent()->SetValueAsVector("PathPoint", Path[CurrentPathPoint]->GetActorLocation());
		}
	}
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}