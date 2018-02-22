// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealMathUtility.h"
#include "AIController.h"
#include "Nomlings.generated.h"

UCLASS()
class SPACEVIKINGS_API ANomlings : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANomlings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float health;
	float maxHealth;
	bool enemy;
	bool moving;
	bool grabbed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		void Grabbed();

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		void Attack();

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		void Melee();

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		void setGrabbed(bool grab);

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		void setMaxHealth(float health);

	UFUNCTION(BlueprintCallable, Category = "Nomlings")
		float getHealth();
	
};
