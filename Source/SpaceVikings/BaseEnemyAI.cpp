// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyAI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "NomlinsAnimInstance.h"
#include "BaseAIController.h"

// Sets default values
ABaseEnemyAI::ABaseEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = maxHealth;

	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->GravityScale = 1.5f;
	moveComp->JumpZVelocity = 620;

	minThreshold = 600.0f * 0.66f;
	maxThreshold = 600.0f * 0.33f;

	mesh = GetMesh();

	leftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	rightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));

	leftArrow->SetupAttachment(mesh);
	rightArrow->SetupAttachment(mesh);

	
}

// Called when the game starts or when spawned
void ABaseEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(timer, this, &ABaseEnemyAI::timerTriggered, 3.0f, true, 0.0f);
	GetWorldTimerManager().SetTimer(attackTimer, this, &ABaseEnemyAI::attack, 0.5f, true, 0.0f);
	GetWorldTimerManager().PauseTimer(attackTimer);
}

void ABaseEnemyAI::attacked()
{
	FCollisionShape sphere = FCollisionShape::MakeSphere(500.0f);
	TArray<FHitResult> outHit;
	GetWorld()->SweepMultiByChannel(outHit, GetActorLocation(), (GetActorForwardVector() * 50) + GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn, sphere);
	
	for (FHitResult hit : outHit)
	{
		AActor *character = hit.GetActor();
		UGameplayStatics::ApplyDamage(character, 1.0f, NULL, this, NULL);
	}
}

void ABaseEnemyAI::predictMove()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (player != nullptr)
	{
		float currentVelocity = player->GetVelocity().Size();

		float predictRange;

		if (currentVelocity < minThreshold)
		{
			predictRange = 0.0f;
		}
		else if (currentVelocity > maxThreshold)
		{
			predictRange = 175.0;
		}
		else
		{
			predictRange = 75.0;
		}

		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), (player->GetActorForwardVector() * predictRange) + player->GetActorLocation());

		SetActorRotation(rotation);
		leftArrow->SetWorldRotation(rotation);
		rightArrow->SetWorldRotation(rotation);
	}
}

bool ABaseEnemyAI::getRandomPointInRadius(const FVector & origin, float radius, FVector & result)
{
	UNavigationSystem* navSys = UNavigationSystem::GetCurrent(GetWorld());

	if(!navSys)
	{
		return false;
	}

	FNavLocation outResult;

	bool bSuccess = navSys->GetRandomPointInNavigableRadius(origin, radius, outResult);

	result = outResult.Location;
	return bSuccess;
}

void ABaseEnemyAI::timerTriggered()
{
	getRandomPointInRadius(GetActorLocation(), 500.0f, location);
}

void ABaseEnemyAI::attack()
{
	if (longRange)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		spawnArea++;

		if ((spawnArea % 2) != 0 && spawnArea < 4)
		{
			if (projectileClass)
			{
				GetWorld()->SpawnActor<Aprojectile>(projectileClass, leftArrow->GetComponentLocation(), leftArrow->GetComponentRotation(), spawnParams);
			}
		}
		else if ((spawnArea % 2) == 0 && spawnArea < 4)
		{
			if(projectileClass)
			{
				GetWorld()->SpawnActor<Aprojectile>(projectileClass, rightArrow->GetComponentLocation(), rightArrow->GetComponentRotation(), spawnParams);
			}
		}
		else
		{
			spawnArea = 0;
		}
	}
	else
	{
		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if(player != nullptr)
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());

			SetActorRotation(rotation);
			canMove = false;

			UNomlinsAnimInstance* nomlinsAnim = Cast<UNomlinsAnimInstance>(GetMesh()->GetAnimInstance());

			if (!nomlinsAnim->attacking)
			{
				LaunchCharacter(GetActorForwardVector() * 500 + FVector(0, 0, 500.0f), true, true);
				nomlinsAnim->attacking = true;
			}
		}
	}
}

void ABaseEnemyAI::death(UObject * toDestroy)
{
	bool isDead = false;


	if (IsPendingKill())
	{
		isDead = false;
	}
	else if (!mesh || !mesh->GetPhysicsAsset())
	{
		isDead = false;
	}
	else
	{
		isDead = true;
	}

	UCharacterMovementComponent* characterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());

	if (!isDead)
	{
		SetLifeSpan(5.0f);
	}
	else
	{
		SetActorTickEnabled(false);
		SetLifeSpan(0.05f);
	}
}

float ABaseEnemyAI::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	health -= Damage;
	return 0.0f;
}

// Called every frame
void ABaseEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health <= 0)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		death(this);
	}

	ABaseAIController* controller = Cast<ABaseAIController>(GetController());
	
	if((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() > range)
	{
		GetWorldTimerManager().PauseTimer(attackTimer);
		controller->MoveToLocation(location, 20.0);
		withInRange = false;
	}
	else if((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() > longAttackRange && longRange && canMove)
	{
		controller->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), longAttackRange - 100);
		
	}
	else if ((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() > attackRange && !longRange)
	{
		controller->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), attackRange - 100);
	}

	if ((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() < longAttackRange && longRange)
	{
		withInRange = true;
		canMove = false;
	}
	else if ((GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).Size() < attackRange)
	{
		attack();
	}
	else
	{
		withInRange = false;
		canMove = true;
	}

	if (longRange && withInRange)
	{
		GetWorldTimerManager().UnPauseTimer(attackTimer);
		predictMove();
	}
}