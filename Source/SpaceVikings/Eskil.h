// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "projectile.h"
#include "Aim.h"
#include "Components/ArrowComponent.h"
#include "Eskil.generated.h"

UCLASS(config = Game)
class SPACEVIKINGS_API AEskil : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AEskil();

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleport")
		bool canTeleport;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		bool useGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool attacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleport")
		FVector teleporterLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim")
		TSubclassOf<UAim> targetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
		TSubclassOf<Aprojectile> projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		class UArrowComponent* arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency")
		float amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float currentHealth;

private:
	UFUNCTION()
		void teleport();

	UFUNCTION()
		void attack();

	UFUNCTION(BlueprintCallable)
		void chainCombo();

	UFUNCTION(BlueprintCallable)
		void comboReset();

	UFUNCTION(BlueprintCallable)
		void usingGun();

	UFUNCTION(BlueprintCallable)
		FVector getInitialLocation();

	UFUNCTION(BlueprintCallable)
		FVector getInitialDirection();

	UFUNCTION()
		void isUsingHammer();

	UFUNCTION(BlueprintCallable)
		void setAttacking();

	UFUNCTION()
		void fireGun();

	UFUNCTION()
		void onOverlapBegin(UPrimitiveComponent * overlapComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool chainAttack;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		int comboCounter;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* attackMontage1;

	UPROPERTY(EditAnywhere, Category = "Attack")
		UAnimMontage* attackMontage2;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		bool targetLock;

	UPROPERTY(EditAnywhere, Category = "Attacking Collision")
		UCapsuleComponent* hammerCollision;

	UPROPERTY(EditAnywhere, Category = "Hammer")
		class UStaticMeshComponent* hammer;

	UPROPERTY(EditAnywhere, Category = "Gun")
		class UStaticMeshComponent* gun;

	UPROPERTY(EditAnywhere, Category = "Hammer")
		class URadialForceComponent* radialForce;

	UPROPERTY(EditAnywhere, Category = "Aim")
		float aimDistance;

	UPROPERTY()
		UAim* target;

	UStaticMesh* hammerMesh;

	UStaticMesh* gunMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
