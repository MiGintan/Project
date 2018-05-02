// Fill out your copyright notice in the Description page of Project Settings.

#include "Create.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ACreate::ACreate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	create = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Create"));
	create->SetupAttachment(RootComponent);

	//DELAYTIME = 4.5f;
}

// Called when the game starts or when spawned
void ACreate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACreate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACreate::actorFracture(const FVector & hitPoint, const FVector & hitDirection)
{
	if (currencyClass)
	{
		create->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		create->AddRadialForce(hitPoint, 300.0, 75000.0f, ERadialImpulseFalloff::RIF_Constant, false);

		int spawnAmount = FMath::RandRange(3, 6);
		for (int i = 0; i < spawnAmount; i++)
		{
			float x = FMath::FRandRange(-50, 50);
			float y = FMath::FRandRange(-50, 50);

			GetWorld()->SpawnActor<ACurrency>(currencyClass, FVector(x + GetActorLocation().X, y + GetActorLocation().Y, GetActorLocation().Z + 50.0f), GetActorRotation(), spawnParams);
		}

		SetActorTickEnabled(false);
		SetLifeSpan(2.0f);
	}
	else
		GLog->Log("Null Class");
}
