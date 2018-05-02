// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Currency.h"
#include "GameFramework/Actor.h"
#include "Components/DestructibleComponent.h"
#include "Create.generated.h"

UCLASS()
class SPACEVIKINGS_API ACreate : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACreate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Create")
		UDestructibleComponent* create;

	UFUNCTION(BlueprintCallable)
		void actorFracture(const FVector& hitPoint, const FVector& hitDirection);

	UPROPERTY(EditDefaultsOnly, Category = "Create")
		TSubclassOf<ACurrency> currencyClass;
};
