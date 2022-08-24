// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ArrowComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CatPathPoint.generated.h"

UCLASS()
class BRACKEYS2022_2_API ACatPathPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACatPathPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* PathDirection;
};
