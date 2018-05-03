// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "projectile.h"
#include "GameFramework/Character.h"
#include "BaseEnemyAI.generated.h"

UCLASS()
class SPACEVIKINGS_API ABaseEnemyAI : public ACharacter
{
	GENERATED_BODY()

private:

	UFUNCTION(BlueprintCallable)
		void attacked();

	UFUNCTION()
		void predictMove();

	UFUNCTION()
		bool getRandomPointInRadius(const FVector& origin, float radius, FVector& result);

	UFUNCTION()
		void timerTriggered();

	UFUNCTION()
		void attack();

	UFUNCTION()
		void death(UObject * toDestroy);

	float minThreshold;

	float maxThreshold;

	FTimerHandle timer;
	FTimerHandle attackTimer;
	FTimerHandle deathTimer;

	FVector location;

	bool withInRange;

	bool canMove;

	int spawnArea;

	USkeletalMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Sets default values for this character's properties
	ABaseEnemyAI();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		float health;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Type")
		bool longRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Range")
		float range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Range")
		float attackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Range")
		float longAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		TSubclassOf<Aprojectile> projectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		class UArrowComponent* leftArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		class UArrowComponent* rightArrow;

	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};