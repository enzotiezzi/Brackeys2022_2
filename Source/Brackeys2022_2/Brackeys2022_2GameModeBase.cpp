// Copyright Epic Games, Inc. All Rights Reserved.


#include "Brackeys2022_2GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerStart.h>
#include <MyPlayerStart.h>
#include <MyCameraActor.h>
#include <MyPlayerController.h>

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
		AMyPlayerController* ChiefChickenController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		AMyPlayerController* BelieverController;

		TArray<AActor*> Controllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), Controllers);
		
		if (Controllers.Num() == 2)
		{
			BelieverController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 1));
		}
		else 
		{
			BelieverController = Cast<AMyPlayerController>(UGameplayStatics::CreatePlayer(GetWorld(), 1));
		}

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

				AActor* MyCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCameraActor::StaticClass());

				ChiefChickenController->SetViewTargetWithBlend(MyCameraActor);
			}
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
		}
	}
}