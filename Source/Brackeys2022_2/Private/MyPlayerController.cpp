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
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetRefFinder(TEXT("/Game/UI/WBP_Lose"));
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetRefFinder(TEXT("/Game/UI/WBP_MainMenu"));

	if(PauseMenuWidgetFinder.Succeeded())
		PauseMenuWidgetRef = PauseMenuWidgetFinder.Class;

	if (GameOverWidgetRefFinder.Succeeded())
		GameOverWidgetRef = GameOverWidgetRefFinder.Class;

	if (MainMenuWidgetRefFinder.Succeeded())
		MainMenuWidgetRef = MainMenuWidgetRefFinder.Class;

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
	SetupGameOverWidget();
	SetupMainMenuWidget();

	if (GetWorld()->GetName() == "TestLevel")
	{
		PauseGame();
		ShowMainMenu();
		PlayIntro();
	}
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
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetRef);

		UButton* ResumeButton = Cast<UButton>(PauseWidget->GetWidgetFromName("ResumeButton"));
		ResumeButton->OnClicked.AddDynamic(this, &AMyPlayerController::ResumeGame);
		
		UButton* RetryButton = Cast<UButton>(PauseWidget->GetWidgetFromName("RetryButton"));
		RetryButton->OnClicked.AddDynamic(this, &AMyPlayerController::RetryLevel);

		UButton* MenuButton = Cast<UButton>(PauseWidget->GetWidgetFromName("MenuButton"));
		MenuButton->OnClicked.AddDynamic(this, &AMyPlayerController::ShowMainMenu);
	}
}

void AMyPlayerController::SetupGameOverWidget()
{
	if (GameOverWidgetRef)
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetRef);
	
		UButton* RetryButton = Cast<UButton>(GameOverWidget->GetWidgetFromName("RetryButton"));
		RetryButton->OnClicked.AddDynamic(this, &AMyPlayerController::RetryLevel);

		UButton* MenuButton = Cast<UButton>(GameOverWidget->GetWidgetFromName("MenuButton"));
		MenuButton->OnClicked.AddDynamic(this, &AMyPlayerController::ShowMainMenu);
	}
}

void AMyPlayerController::SetupMainMenuWidget()
{
	if (MainMenuWidgetRef)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetRef);

		UButton* PlayButton = Cast<UButton>(MainMenuWidget->GetWidgetFromName("PlayButton"));
		PlayButton->OnClicked.AddDynamic(this, &AMyPlayerController::PlayGame);

		UButton* CreditsButton = Cast<UButton>(MainMenuWidget->GetWidgetFromName("CreditsButton"));
		CreditsButton->OnClicked.AddDynamic(this, &AMyPlayerController::ShowCredits);

		UButton* QuitButton = Cast<UButton>(MainMenuWidget->GetWidgetFromName("QuitButton"));
		QuitButton->OnClicked.AddDynamic(this, &AMyPlayerController::QuitGame);

		StartAnim = GetAnimation(FText::FromString("StartAnim"));

		FWidgetAnimationDynamicEvent StartAnimAnimationEvent;
		StartAnimAnimationEvent.BindDynamic(this, &AMyPlayerController::OnStartAnimFinished);

		MainMenuWidget->BindToAnimationFinished(StartAnim, StartAnimAnimationEvent);

		ConstantAnim = GetAnimation(FText::FromString("ConstantAnim"));
	}
}

void AMyPlayerController::OnStartAnimFinished()
{
	if (MainMenuSound)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), MainMenuSound);
	}

	if(ConstantAnim)
	{
		MainMenuWidget->PlayAnimation(ConstantAnim,0, 0);
	}

	SetShowMouseCursor(true);

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetWidgetToFocus(MainMenuWidget->GetWidgetFromName("PlayButton")->TakeWidget());

	SetInputMode(InputMode);
}


void AMyPlayerController::PlayIntro()
{
	if (StartAnim) 
	{
		SetShowMouseCursor(false);

		MainMenuWidget->PlayAnimation(StartAnim);
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

void AMyPlayerController::ShowMainMenu()
{
	if (!MainMenuWidget->IsInViewport()) 
	{
		if (PauseWidget->IsInViewport())
			PauseWidget->RemoveFromViewport();

		if (GameOverWidget->IsInViewport())
			GameOverWidget->RemoveFromViewport();

		MainMenuWidget->AddToViewport(1);

		UGameplayStatics::SetGamePaused(GetWorld(), true);

		SetShowMouseCursor(true);

		MainMenuWidget->StopAllAnimations();

		if (ConstantAnim)
		{
			MainMenuWidget->PlayAnimation(ConstantAnim, 0, 0);
		}

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetWidgetToFocus(MainMenuWidget->GetWidgetFromName("PlayButton")->TakeWidget());

		SetInputMode(InputMode);
	}
}

void AMyPlayerController::CallGameOver()
{
	if (!GameOverWidget->IsInViewport())
	{
		GameOverWidget->AddToViewport();

		UGameplayStatics::SetGamePaused(GetWorld(), true);

		SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetWidgetToFocus(GameOverWidget->GetWidgetFromName("RetryButton")->TakeWidget());

		SetInputMode(InputMode);
	}
}

void AMyPlayerController::RetryLevel()
{	
	GameOverWidget->RemoveFromViewport();

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	SetShowMouseCursor(false);

	SetInputMode(FInputModeGameOnly());

	UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()));
}

void AMyPlayerController::PlayGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	SetShowMouseCursor(false);

	SetInputMode(FInputModeGameOnly());

	UGameplayStatics::OpenLevel(GetWorld(), FName("Level1"));
}

void AMyPlayerController::ShowCredits() 
{
	GEngine->AddOnScreenDebugMessage(rand(), 1, FColor::Red, "Credits");
}

void AMyPlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}

void AMyPlayerController::MakeNoise(APawn* PawnToMakeNoise)
{
	if (APlayerCharacter* CurrentCharacter = Cast<APlayerCharacter>(PawnToMakeNoise))
	{
		CurrentCharacter->MakeNoise();
	}

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

UWidgetAnimation* AMyPlayerController::GetAnimation(FText AnimationName)
{
	UProperty* Prop = MainMenuWidget->GetClass()->PropertyLink;

	while (Prop)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjectProp = Cast<UObjectProperty>(Prop);

			if (ObjectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Object = ObjectProp->GetObjectPropertyValue_InContainer(MainMenuWidget);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Object);

				if (WidgetAnim)
				{
					if (WidgetAnim->GetDisplayName().CompareTo(AnimationName) == 0)
						return WidgetAnim;
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}

	return nullptr;
}