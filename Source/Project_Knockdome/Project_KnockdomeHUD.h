// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Project_KnockdomeHUD.generated.h"

UCLASS()
class AProject_KnockdomeHUD : public AHUD
{
	GENERATED_BODY()

public:
	AProject_KnockdomeHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

