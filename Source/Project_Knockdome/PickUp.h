// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UCLASS()
class PROJECT_KNOCKDOME_API APickUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* PickUpRoot;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* PickUpHitBox;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float YawValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float RollValue;

	UFUNCTION()
		void OnPlayerEnterPickUpItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
