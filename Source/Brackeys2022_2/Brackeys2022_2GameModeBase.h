// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <PlayerCharacter.h>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Brackeys2022_2GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BRACKEYS2022_2_API ABrackeys2022_2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABrackeys2022_2GameModeBase();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Reference")
	TSubclassOf<APlayerCharacter> ChiefChickenRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Reference")
	TSubclassOf<APlayerCharacter> BelieverRef;
};
