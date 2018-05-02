// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveVector.h"
#include "Currency.generated.h"

UCLASS()
class SPACEVIKINGS_API ACurrency : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurrency();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float zAmp;
	float xAmp;
	float yAmp;
	float time;

	bool animStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
		UCurveVector * curve;
};
