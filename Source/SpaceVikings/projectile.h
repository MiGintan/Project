// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "projectile.generated.h"

UCLASS()
class SPACEVIKINGS_API Aprojectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aprojectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		class USphereComponent* sphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		class UStaticMeshComponent* mesh;

	UStaticMesh* staticMesh;

private:
	UFUNCTION(BlueprintCallable, Category = "Default")
		void onOverlapBegin(class UPrimitiveComponent* overlapComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};