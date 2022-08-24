// Fill out your copyright notice in the Description page of Project Settings.


#include "CatPathPoint.h"

// Sets default values
ACatPathPoint::ACatPathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PathDirection = CreateDefaultSubobject<UArrowComponent>("PathDirection");

	PathDirection->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACatPathPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatPathPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}