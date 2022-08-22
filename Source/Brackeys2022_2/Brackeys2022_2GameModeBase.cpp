// Copyright Epic Games, Inc. All Rights Reserved.


#include "Brackeys2022_2GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerStart.h>
#include <MyPlayerStart.h>

ABrackeys2022_2GameModeBase::ABrackeys2022_2GameModeBase()
{

}

void ABrackeys2022_2GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> PlayersStart;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyPlayerStart::StaticClass(), PlayersStart);

	if (PlayersStart.Num() == 2)
	{
		APlayerController* ChiefChickenController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APlayerController* BelieverController = UGameplayStatics::CreatePlayer(GetWorld(), 1);

		if (ChiefChickenRef)
		{
			FVector ChiefChickenLocation = PlayersStart[0]->GetActorLocation();
			FRotator ChiefChickenRotation = PlayersStart[0]->GetActorRotation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = ChiefChickenController;

			APlayerCharacter* ChiefChicken = GetWorld()->SpawnActor<APlayerCharacter>(ChiefChickenRef, ChiefChickenLocation, ChiefChickenRotation, SpawnParameters);
		
			if (ChiefChicken)
			{
				ChiefChickenController->Possess(ChiefChicken);
			}

			PlayersStart[0]->Destroy();
		}

		if (BelieverRef)
		{
			FVector BelieverLocation = PlayersStart[1]->GetActorLocation();
			FRotator BelieverRotation = PlayersStart[1]->GetActorRotation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = BelieverController;

			APlayerCharacter* Believer = GetWorld()->SpawnActor<APlayerCharacter>(BelieverRef, BelieverLocation, BelieverRotation, SpawnParameters);

			if (Believer)
			{
				BelieverController->Possess(Believer);
			}

			PlayersStart[1]->Destroy();
		}
	}
}