// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_KnockdomeGameMode.h"
#include "Project_KnockdomeHUD.h"
#include "Project_KnockdomeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_KnockdomeGameMode::AProject_KnockdomeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AProject_KnockdomeHUD::StaticClass();
}
