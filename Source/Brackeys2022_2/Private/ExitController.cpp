// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitController.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <MyPlayerController.h>

// Sets default values
AExitController::AExitController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuWidgetFinder(TEXT("/Game/UI/WBP_Win"));

	if (PauseMenuWidgetFinder.Succeeded())
		WinWidgetRef = PauseMenuWidgetFinder.Class;
}

// Called when the game starts or when spawned
void AExitController::BeginPlay()
{
	Super::BeginPlay();

	SetupWinWidget();
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
		if (!WinWidget->IsInViewport())
			WinWidget->AddToViewport();

		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(ChiefChicken->GetController()))
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			PlayerController->SetShowMouseCursor(true);

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			InputMode.SetWidgetToFocus(WinWidget->GetWidgetFromName("NextLvlButton")->TakeWidget());

			PlayerController->SetInputMode(InputMode);
		}
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

void AExitController::SetupWinWidget()
{
	if (WinWidgetRef)
	{
		WinWidget = CreateWidget<UUserWidget>(GetWorld(), WinWidgetRef);

		if (WinWidget)
		{
			UButton* NextLevelButton = Cast<UButton>(WinWidget->GetWidgetFromName("NextLvlButton"));

			NextLevelButton->OnClicked.AddDynamic(this, &AExitController::GoToNextLevel);
		}
	}
}

void AExitController::GoToNextLevel()
{
	if (WinWidget->IsInViewport())
		WinWidget->RemoveFromViewport();

	if (!NextLevelName.IsNone())
		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
}