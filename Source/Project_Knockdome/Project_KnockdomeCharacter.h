// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Project_Knockdome_Push_Ability.h"
#include "Project_KnockdomeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AProject_KnockdomeCharacter : public ACharacter
{
	GENERATED_BODY()

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	int shotgunAmmo{ 5 };

	

public:
	AProject_KnockdomeCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class AProject_KnockdomeProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AProject_KnockdomeProjectile> shotgunProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AProject_KnockdomeProjectile> peaShooterProjectile;

	// Ability class to spawn
	UPROPERTY(EditAnywhere, Category = Projectile)
	UClass* AbilityClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	int weaponIndex;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	int ammoCount{ 0 };
	
	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	bool canFire{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int playerIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage)
	float playerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float abilityCharge{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int playerScore{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float bulletBoost { 0.f };

protected:
	
	

	// Uses the knockback ability
	void useAbility();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = Hits)
	void OnHit(FVector enemyVelocity, float bulletDamage);

	/*UFUNCTION(BlueprintCallable, Category = Hits)
	void onAbilityHit(FVector enemyVelocity);*/
	
	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void OnFire();
};

