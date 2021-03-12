// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_Knockdome_Push_Ability.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AProject_Knockdome_Push_Ability : public AActor
{
	GENERATED_BODY()

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UBoxComponent* CollisionComp;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	AProject_Knockdome_Push_Ability();

	UPROPERTY(BlueprintReadWrite, Category = "Index")
		int abilityPlayerIndex{ 0 };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
