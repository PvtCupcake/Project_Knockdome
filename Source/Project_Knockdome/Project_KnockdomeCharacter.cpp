// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_KnockdomeCharacter.h"
#include "Project_KnockdomeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Math/UnrealMathUtility.h"
#include "Project_Knockdome_Push_Ability.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AProject_KnockdomeCharacter

AProject_KnockdomeCharacter::AProject_KnockdomeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void AProject_KnockdomeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	Mesh1P->SetHiddenInGame(false, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProject_KnockdomeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProject_KnockdomeCharacter::OnFire);

	// Bind ability event
	PlayerInputComponent->BindAction("UseAbility", IE_Pressed, this, &AProject_KnockdomeCharacter::useAbility);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AProject_KnockdomeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProject_KnockdomeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProject_KnockdomeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProject_KnockdomeCharacter::LookUpAtRate);
}

void AProject_KnockdomeCharacter::OnHit(FVector enemyVelocity, float bulletDamage)
{
	playerDamage += bulletDamage;

	FVector launchVelocity = enemyVelocity;
	launchVelocity = launchVelocity * playerDamage;
	launchVelocity = launchVelocity + FVector(0.f, 0.f, 300.0f);
	this->LaunchCharacter(launchVelocity, false, false);
}

void AProject_KnockdomeCharacter::onAbilityHit(FVector enemyVelocity)
{
	FVector launchVelocity = enemyVelocity;
	launchVelocity = launchVelocity * (playerDamage * 2);
	launchVelocity = launchVelocity + FVector(0.f, 0.f, 300.0f);
	this->LaunchCharacter(launchVelocity, false, false);
}

void AProject_KnockdomeCharacter::ChangeWeapon()
{
	//Set ammo
}

void AProject_KnockdomeCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (canFire == true)
			{
				canFire = false;
				if (ammoCount > 0)
				{
					if (weaponIndex == 3)
					{
						if (shotgunAmmo > 0)				// The shotgunBow shoots shotgun pellets while the shotgun has ammo
						{
							for (int i{ 0 }; i < 7; i++)
							{
								FMath tempMath;
								FVector coneDirection;
								coneDirection = GetControlRotation().Vector();
								float coneHalfAngle = 5.f;													// Changes the spread of the shotgun, represents the radius of the circle
								float coneHalfAngleRad = tempMath.DegreesToRadians(coneHalfAngle);
								FVector randomCone = tempMath.VRandCone(coneDirection, coneHalfAngleRad);	// Sets a random vector within the given cone as the velocity for the shotgun pellets

								const FRotator spawnRotation = randomCone.Rotation(); 
								const FVector spawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + spawnRotation.RotateVector(GunOffset);
								FActorSpawnParameters ActorSpawnParams;
								ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

								AProject_KnockdomeProjectile* spawnedProjectile = World->SpawnActor<AProject_KnockdomeProjectile>(ProjectileClass, spawnLocation, spawnRotation, ActorSpawnParams);

								spawnedProjectile->projectilePlayerIndex = playerIndex;
							}
							shotgunAmmo--;
						}
						// When the shotgun is out of ammo the shotgunBow shoots the shotgun as a projectile instead
						else
						{
							const FRotator SpawnRotation = GetControlRotation();
							// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
							const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

							//Set Spawn Collision Handling Override
							FActorSpawnParameters ActorSpawnParams;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

							// spawn the projectile at the muzzle
							AProject_KnockdomeProjectile* spawnedProjectile = World->SpawnActor<AProject_KnockdomeProjectile>(shotgunProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
							
							spawnedProjectile->projectilePlayerIndex = playerIndex;
							shotgunAmmo = 5;
						}
					}

					if (weaponIndex == 1)
					{
						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the projectile at the muzzle
						AProject_KnockdomeProjectile* spawnedProjectile = World->SpawnActor<AProject_KnockdomeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

						spawnedProjectile->projectilePlayerIndex = playerIndex;
					}

					else
					{
						const FRotator SpawnRotation = GetControlRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						// spawn the projectile at the muzzle
						AProject_KnockdomeProjectile* spawnedProjectile = World->SpawnActor<AProject_KnockdomeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

						spawnedProjectile->projectilePlayerIndex = playerIndex;
					}
					ammoCount--;
					if (ammoCount == 0)
						weaponIndex = 0;
				}
				else
				{
					const FRotator SpawnRotation = GetControlRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					// spawn the projectile at the muzzle
					AProject_KnockdomeProjectile* spawnedProjectile = World->SpawnActor<AProject_KnockdomeProjectile>(peaShooterProjectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
					
					spawnedProjectile->projectilePlayerIndex = playerIndex;
				}

				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}

				// try and play a firing animation if specified
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
					if (AnimInstance != nullptr)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
		}
	}
}

void AProject_KnockdomeCharacter::useAbility()
{
	if (abilityCharge >= 1.0f)
	{
		if (AbilityClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FRotator spawnRot = GetControlRotation();
				// The pushability projectile should also spawn at the muzzle so it does not hit out character
				const FVector spawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + spawnRot.RotateVector(GunOffset);

				// Set spawn parameters
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// Spawn projectile
				World->SpawnActor<AProject_Knockdome_Push_Ability>(AbilityClass, spawnLocation, spawnRot, ActorSpawnParams);
				
				// Reset abilityCharge
				abilityCharge = 0.f;
			}
		}
	}
}

void AProject_KnockdomeCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AProject_KnockdomeCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AProject_KnockdomeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProject_KnockdomeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}