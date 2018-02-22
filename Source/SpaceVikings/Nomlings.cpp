// Fill out your copyright notice in the Description page of Project Settings.

#include "Nomlings.h"
#include "Engine.h"
#include "NavigationSystem.h"

// Sets default values
ANomlings::ANomlings()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = maxHealth;
}

// Called when the game starts or when spawned
void ANomlings::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANomlings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (grabbed == true)
	{
		Grabbed();
	}
	else
	{
		if (health <= 0.0f)
		{
			LaunchCharacter((GetActorForwardVector() * 50) + FVector(0, 0, 70), false, false);
			Destroy();
		}
		else
		{
			if (enemy == true)
			{
				Attack();
			}
			else
			{
				if (moving == true)
				{
					
				}
			}
		}

		if ((GetActorLocation() - GetWorld()->GetFirstPlayerController()->K2_GetActorLocation()).Size() > 1000)
			enemy = false;
		else
			enemy = true;
	}
}

void ANomlings::Grabbed()
{
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorLocation(FMath::VInterpTo(FVector(GetActorLocation().X, GetActorLocation().Y, (GetWorld()->GetFirstPlayerController()->K2_GetActorLocation() + FVector(0, 0, 50)).Z),
		GetWorld()->GetFirstPlayerController()->K2_GetActorLocation(), GetWorld()->DeltaTimeSeconds, 1), false, false);

	SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), FVector().ZeroVector, GetWorld()->DeltaTimeSeconds, 1));

	if (FMath::VInterpTo(GetActorScale3D(), FVector().ZeroVector, GetWorld()->DeltaTimeSeconds, 1) == FVector().ZeroVector)
	{
		Destroy();
	}
}

void ANomlings::Attack()
{

}

void ANomlings::Melee()
{

}

void ANomlings::setGrabbed(bool grab)
{
	grabbed = grab;
}

void ANomlings::setMaxHealth(float health)
{
	maxHealth = health;
}

float ANomlings::getHealth()
{
	return 0.0f;
}
