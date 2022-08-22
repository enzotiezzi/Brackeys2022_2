// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitArea.h"
#include <PlayerCharacter.h>

AExitArea::AExitArea()
{

}

void AExitArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ExitController)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
		{
			ExitController->NotifyChickenArrived(Player);
		}
	}
}

void AExitArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (ExitController)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
		{
			ExitController->NotifyChickenGotOut(Player);
		}
	}
}
