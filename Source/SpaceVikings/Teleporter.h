// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Teleporter.generated.h"

UCLASS()
class SPACEVIKINGS_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
		float radius;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* sphereCollision;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
		ATeleporter* teleporter;

	UFUNCTION()
		void onOverlapBegin(class UPrimitiveComponent* overlapComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void onOverlapEnd(class UPrimitiveComponent* overlapComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
