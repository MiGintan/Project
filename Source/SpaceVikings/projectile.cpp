// Fill out your copyright notice in the Description page of Project Settings.

#include "projectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
Aprojectile::Aprojectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	mesh->SetStaticMesh(staticMesh);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &Aprojectile::onOverlapBegin);
	SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void Aprojectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void Aprojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void Aprojectile::onOverlapBegin(UPrimitiveComponent * overlapComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	GLog->Log("Overlaped");
	UGameplayStatics::ApplyDamage(otherActor, 1.5f, NULL, this, NULL);
	//radialForce->FireImpulse();
	Destroy();
}

