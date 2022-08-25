// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <MyPlayerController.h>
#include <UMG/Public/Blueprint/UserWidget.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    SoundWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("SoundWidgetComponent");

	SpringArmComponent->SetupAttachment(GetMesh());
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    SoundWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void APlayerCharacter::MakeNoise()
{
	if (SoundWidgetComponent->GetWidget())
	{
        UWidgetAnimation* ShowAnim = GetAnimation(FText::FromString("ShowAnim"));

        if (ShowAnim)
        {
            FWidgetAnimationDynamicEvent AnimationFinishEvent;
            AnimationFinishEvent.BindDynamic(this, &APlayerCharacter::OnAnimationFinished);

            SoundWidgetComponent->SetVisibility(true);
            SoundWidgetComponent->GetWidget()->BindToAnimationFinished(ShowAnim, AnimationFinishEvent);
            SoundWidgetComponent->GetWidget()->PlayAnimation(ShowAnim);
        }
	}
}

UWidgetAnimation* APlayerCharacter::GetAnimation(FText AnimationName)
{
    UProperty* Prop = SoundWidgetComponent->GetWidget()->GetClass()->PropertyLink;

    while (Prop)
    {
        if (Prop->GetClass() == UObjectProperty::StaticClass())
        {
            UObjectProperty* ObjectProp = Cast<UObjectProperty>(Prop);

            if (ObjectProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject* Object = ObjectProp->GetObjectPropertyValue_InContainer(SoundWidgetComponent->GetWidget());

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

void APlayerCharacter::OnAnimationFinished()
{
    SoundWidgetComponent->SetVisibility(false);
}