// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_Knockdome_Push_Ability.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AProject_Knockdome_Push_Ability::AProject_Knockdome_Push_Ability()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(5.0f, 5.0f, 5.0f));
	CollisionComp->BodyInstance.SetCollisionProfileName("Ability");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AProject_KnockdomeProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 1 second by defualt
	InitialLifeSpan = 100.0f;
}

// Called when the game starts or when spawned
void AProject_Knockdome_Push_Ability::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProject_Knockdome_Push_Ability::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

