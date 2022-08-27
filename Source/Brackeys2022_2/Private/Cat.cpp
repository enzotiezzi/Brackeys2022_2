// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"
#include <CatAIController.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <UMG/Public/Blueprint/UserWidget.h>
#include <MyPlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <Components/TextBlock.h>

// Sets default values
ACat::ACat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");

    WidgetComponent->SetupAttachment(GetRootComponent());
    SphereComponent->SetupAttachment(GetRootComponent());

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACat::OnSphereComponentBeginOverlap);
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

		AIController->SetHearingRange(HearingRange);
	}
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UWidgetAnimation* ACat::GetAnimation(FText AnimationName)
{
    UProperty* Prop = WidgetComponent->GetWidget()->GetClass()->PropertyLink;

    while (Prop)
    {
        if (Prop->GetClass() == UObjectProperty::StaticClass())
        {
            UObjectProperty* ObjectProp = Cast<UObjectProperty>(Prop);

            if (ObjectProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject* Object = ObjectProp->GetObjectPropertyValue_InContainer(WidgetComponent->GetWidget());

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

void ACat::NotifySense(FText TextSense)
{
    UWidgetAnimation* ShowAnim = GetAnimation(FText::FromString("ShowAnim"));

    if (ShowAnim)
    {
        FWidgetAnimationDynamicEvent AnimationFinishEvent;
        AnimationFinishEvent.BindDynamic(this, &ACat::OnAnimationFinished);
        
        WidgetComponent->GetWidget()->StopAllAnimations();
        WidgetComponent->SetVisibility(true);
        WidgetComponent->GetWidget()->BindToAnimationFinished(ShowAnim, AnimationFinishEvent);

        UTextBlock* TextBlock = Cast<UTextBlock>(WidgetComponent->GetWidget()->GetWidgetFromName("TextBlock_0"));

        if (TextBlock)
            TextBlock->SetText(TextSense);

        if (AnimationTimerHandle.IsValid())
            GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);

        GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, [this, ShowAnim, TextSense]()
            {
                WidgetComponent->GetWidget()->PlayAnimation(ShowAnim);
            }, GetWorld()->GetDeltaSeconds(), false);
    }
}

void ACat::OnAnimationFinished()
{
    WidgetComponent->SetVisibility(false);
}

void ACat::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

        PlayerController->CallGameOver();
    }

    ACatAIController* CatAIController = Cast<ACatAIController>(GetController());

    GetWorld()->GetTimerManager().ClearTimer(CatAIController->BackToPatrolTimer);
}