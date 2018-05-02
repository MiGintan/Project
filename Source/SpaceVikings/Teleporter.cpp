// Fill out your copyright notice in the Description page of Project Settings.

#include "Teleporter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Eskil.h"

// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere component"));
	sphereCollision->InitSphereRadius(radius);
	sphereCollision->SetCollisionProfileName("Trigger");
	RootComponent = sphereCollision;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(RootComponent);

	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::onOverlapBegin);
	sphereCollision->OnComponentEndOverlap.AddDynamic(this, &ATeleporter::onOverlapEnd);
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleporter::onOverlapBegin(UPrimitiveComponent * overlapComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (otherActor->ActorHasTag("Player"))
	{
		AEskil* eskil = Cast<AEskil>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		eskil->canTeleport = true;
		eskil->teleporterLocation = teleporter->GetActorLocation();
	}
}

void ATeleporter::onOverlapEnd(UPrimitiveComponent * overlapComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex)
{
	if (otherActor->ActorHasTag("Player"))
	{
		AEskil* eskil = Cast<AEskil>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		eskil->canTeleport = false;
	}
}
