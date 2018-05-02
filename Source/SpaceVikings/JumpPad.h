// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "JumpPad.generated.h"

UCLASS()
class SPACEVIKINGS_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		AActor* apex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool show;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		float distance;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
		FVector locationStart;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
		FVector locationApex;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
		FVector launchVector;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
		float loopTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USphereComponent* sphereCollision;

private:
	UFUNCTION(BlueprintCallable, Category = "Default")
		void setWorldLocationApex();

	UFUNCTION(BlueprintCallable, Category = "Default")
		void setLaunchVector();

	UFUNCTION(BlueprintCallable, Category = "Default")
		void setWorldLocation();

	UFUNCTION(BlueprintCallable, Category = "Default")
		void onOverlapBegin(class UPrimitiveComponent* overlapComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
