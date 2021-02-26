// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Components/BoxComponent.h"
//#include <Runtime\Engine\Private\Components\BoxComponent.cpp>

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Universal Configs
	PitchValue = 0.f;
	YawValue = 0.5f;
	RollValue = 0.f;
	XValue = 0.f;
	YValue = 0.f;
	ZValue = 10.f;

	//Component Set Up
	PickUpRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpRoot"));
	RootComponent = PickUpRoot;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	

	PickUpHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpItem"));
	PickUpHitBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickUpHitBox->SetGenerateOverlapEvents(true);
	PickUpHitBox->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnPlayerEnterPickUpItem);
	


}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();

	PickUpMesh->AttachToComponent(PickUpRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PickUpHitBox->AttachToComponent(PickUpRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//on every frame change rotation for actor
	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);
	FQuat QuatRotation = FQuat(NewRotation);
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	// on every frame change location for floating actor
	FVector NewLocation = GetActorLocation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.X += DeltaHeight * XValue;
	NewLocation.Y += DeltaHeight * YValue;
	NewLocation.Z += DeltaHeight * ZValue;

	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);

}

void APickUp::OnPlayerEnterPickUpItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//Destroy();

}

