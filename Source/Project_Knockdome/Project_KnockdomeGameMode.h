// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_KnockdomeGameMode.generated.h"

UCLASS(minimalapi)
class AProject_KnockdomeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject_KnockdomeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = players)
	int maxPlayerCount = 2;
};



