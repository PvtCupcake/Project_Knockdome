// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_Knockdome_Push_Ability.generated.h"

UCLASS()
class PROJECT_KNOCKDOME_API AProject_Knockdome_Push_Ability : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProject_Knockdome_Push_Ability();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
