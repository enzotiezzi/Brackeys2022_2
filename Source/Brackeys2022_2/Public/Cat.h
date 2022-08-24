// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CatPathPoint.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cat.generated.h"

UCLASS()
class BRACKEYS2022_2_API ACat : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path")
	TArray<ACatPathPoint*> Path;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Path")
	int CurrentPathPoint;
};
