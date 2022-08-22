// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitController.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AExitController::AExitController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AExitController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExitController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitController::CheckWinCondition()
{
	if (ChiefChicken && Believer)
	{
		GEngine->AddOnScreenDebugMessage(rand(), 1, FColor::Yellow, "YOU WIN");
	
		if(!NextLevelName.IsNone())
			UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
}

void AExitController::NotifyChickenArrived(APlayerCharacter* Player)
{
	switch (Player->PlayerType)
	{
		case EPlayerType::ChiefChicken:
		{
			ChiefChicken = Player;
		}
		break;

		case EPlayerType::Believer:
		{
			Believer = Player;
		}
		break;
	}

	CheckWinCondition();
}

void AExitController::NotifyChickenGotOut(APlayerCharacter* Player)
{
	switch (Player->PlayerType)
	{
		case EPlayerType::ChiefChicken:
		{
			ChiefChicken = nullptr;
		}
		break;

		case EPlayerType::Believer:
		{
			Believer = nullptr;
		}
		break;
	}
}