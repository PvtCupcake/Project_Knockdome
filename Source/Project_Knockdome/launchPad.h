// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "launchPad.generated.h"

UCLASS()
class PROJECT_KNOCKDOME_API AlaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AlaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = launch)
	void launchPlayer(ACharacter *collidingPlayer);

private:
	UPROPERTY(EditAnywhere, Category = launch)
	FVector launchVelocity;
};
