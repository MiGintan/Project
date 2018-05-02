// Fill out your copyright notice in the Description page of Project Settings.

#include "Currency.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Eskil.h"

// Sets default values
ACurrency::ACurrency()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	yAmp = UKismetMathLibrary::RandomFloatInRange(-150, 150);
	xAmp = UKismetMathLibrary::RandomFloatInRange(-150, 150);
	zAmp = UKismetMathLibrary::RandomFloatInRange(0, 200);
}

// Called when the game starts or when spawned
void ACurrency::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACurrency::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (animStarted)
	{
		if ((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() < 25.0f)
		{
			AEskil* player = Cast<AEskil>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			player->amount++;

			this->SetActorEnableCollision(false);
			Destroy();
		}
		else
		{
			SetActorLocation(UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), DeltaTime, 1100.0f), false, nullptr, ETeleportType::TeleportPhysics);
			time += DeltaTime;
			RootComponent->SetWorldLocation(GetActorLocation() + FVector(curve->GetVectorValue(FMath::Clamp(time*2, 0.0f, 1.0f)).X * xAmp, curve->GetVectorValue(FMath::Clamp(time * 2, 0.0f, 1.0f)).Y * yAmp, curve->GetVectorValue(FMath::Clamp(time * 2, 0.0f, 1.0f)).Z * zAmp));
		}
	}
	else
	{
		animStarted = (GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() < 500.0f;
		
		if (animStarted)
		{
			SetActorLocation(GetActorLocation());
		}
	}
}
