// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ExitController.h>
#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ExitArea.generated.h"

/**
 * 
 */
UCLASS()
class BRACKEYS2022_2_API AExitArea : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AExitArea();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AExitController* ExitController;
};
