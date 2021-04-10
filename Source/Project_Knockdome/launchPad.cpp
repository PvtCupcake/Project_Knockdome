// Fill out your copyright notice in the Description page of Project Settings.


#include "launchPad.h"
#include "GameFramework/Character.h"

// Sets default values
AlaunchPad::AlaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AlaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AlaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AlaunchPad::launchPlayer(ACharacter *collidingPlayer)
{
	collidingPlayer->LaunchCharacter(launchVelocity, true, true);
}

