// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpPad.h"
#include "Eskil.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere component"));
	sphereCollision->InitSphereRadius(75.0f);
	sphereCollision->SetCollisionProfileName("Trigger");
	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::onOverlapBegin);
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
	setWorldLocationApex();
	setWorldLocation();
	setLaunchVector();
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJumpPad::setWorldLocationApex()
{
	if(apex != nullptr)
	{
		locationApex = apex->GetActorLocation();
	}
	else
	{
		GLog->Log("Apex not set");
	}
}

void AJumpPad::setLaunchVector()
{
	launchVector.X = (locationApex.X - locationStart.X) / (sqrtf((locationApex.Z - locationStart.Z) * 980 * 2) / 980);
	launchVector.Y = (locationApex.Y - locationStart.Y) / (sqrtf((locationApex.Z - locationStart.Z) * 980 * 2) / 980);
	launchVector.Z = sqrtf((locationApex.Z - locationStart.Z) * 980 * 2);
}

void AJumpPad::setWorldLocation()
{
	locationStart = (sphereCollision->GetRelativeTransform() * RootComponent->GetComponentTransform()).GetLocation();
}

void AJumpPad::onOverlapBegin(class UPrimitiveComponent* overlapComp, class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->LaunchCharacter(launchVector * sqrtf(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetCharacterMovement()->GetGravityZ() / -980.0f), true, true);
	}
}