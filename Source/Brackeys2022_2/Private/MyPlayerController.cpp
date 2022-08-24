// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>
#include <Camera/CameraComponent.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <Components/Button.h>
#include <Components/PawnNoiseEmitterComponent.h>

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuWidgetFinder(TEXT("/Game/UI/WBP_PauseMenu"));

	if(PauseMenuWidgetFinder.Succeeded())
		PauseMenuWidgetRef = PauseMenuWidgetFinder.Class;

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("NoiseEmitterComponent");
}

void AMyPlayerController::OnPossess(APawn* NewPawn)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(NewPawn))
	{
		if (PlayerCharacter->PlayerType == EPlayerType::Believer)
		{
			PlayerCharacter->SpringArmComponent->DestroyComponent();
			PlayerCharacter->CameraComponent->DestroyComponent();
		}
	}

	Super::OnPossess(NewPawn);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupPauseMenuWidget();
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("LeftAnalogForwardMovement", this, &AMyPlayerController::MoveForwardLeftAnalog);
	InputComponent->BindAxis("LeftAnalogSideMovement", this, &AMyPlayerController::MoveSidesLeftAnalog);
	InputComponent->BindAxis("RightAnalogForwardMovement", this, &AMyPlayerController::MoveForwardRightAnalog);
	InputComponent->BindAxis("RightAnalogSideMovement", this, &AMyPlayerController::MoveSidesRightAnalog);

	InputComponent->BindAction("Pause", IE_Pressed, this, &AMyPlayerController::PauseGame);
	InputComponent->BindAction("Noise", IE_Pressed, this, &AMyPlayerController::MakeNoiseCurrentPawn);
	InputComponent->BindAction("BelieverNoise", IE_Pressed, this, &AMyPlayerController::BelieverMakeNoise);
}

void AMyPlayerController::MoveForward(float AxisValue, APawn* PawnToMove)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		PawnToMove->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveSides(float AxisValue, APawn* PawnToMove)
{
	if (AxisValue != 0)
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation(0, ControllerRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		PawnToMove->AddMovementInput(Direction, AxisValue);
	}
}

void AMyPlayerController::MoveForwardLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MoveForward(AxisValue, CurrentCharacter);
	}
}

void AMyPlayerController::MoveSidesLeftAnalog(float AxisValue)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		MoveSides(AxisValue, CurrentCharacter);
	}
}

void AMyPlayerController::MoveForwardRightAnalog(float AxisValue)
{
	APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn());

	if (APlayerCharacter* Player2Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()))
	{
		if (Player2Character->PlayerType == EPlayerType::Believer && CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveForward(AxisValue, Player2Character);
		}
	}
}

void AMyPlayerController::MoveSidesRightAnalog(float AxisValue)
{
	APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn());

	if (APlayerCharacter* Player2Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()))
	{
		if (Player2Character->PlayerType == EPlayerType::Believer && CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MoveSides(AxisValue, Player2Character);
		}
	}
}

void AMyPlayerController::SetupPauseMenuWidget()
{
	if (PauseMenuWidgetRef)
	{
		if (!PauseWidget)
		{
			PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetRef);

			UButton* ResumeButton = Cast<UButton>(PauseWidget->GetWidgetFromName("ResumeButton"));
			ResumeButton->OnClicked.AddDynamic(this, &AMyPlayerController::ResumeGame);
		}
	}
}

void AMyPlayerController::PauseGame()
{
	if (PauseWidget)
	{
		if (!PauseWidget->IsInViewport())
			PauseWidget->AddToViewport();
		
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		SetShowMouseCursor(true);

		FInputModeUIOnly InputMode; 
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetWidgetToFocus(PauseWidget->GetWidgetFromName("ResumeButton")->TakeWidget());
		
		SetInputMode(InputMode);
	}
}

void AMyPlayerController::ResumeGame()
{
	if (PauseWidget->IsInViewport())
	{
		PauseWidget->RemoveFromViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void AMyPlayerController::MakeNoise(APawn* PawnToMakeNoise)
{
	NoiseEmitterComponent->MakeNoise(this, 1, PawnToMakeNoise->GetActorLocation());
}

void AMyPlayerController::MakeNoiseCurrentPawn()
{
	MakeNoise(GetPawn());
}

void AMyPlayerController::BelieverMakeNoise()
{
	APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(GetPawn());

	if (APlayerCharacter* Player2Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()))
	{
		if (Player2Character->PlayerType == EPlayerType::Believer && CurrentCharacter->PlayerType == EPlayerType::ChiefChicken)
		{
			MakeNoise(Player2Character);
		}
	}
}