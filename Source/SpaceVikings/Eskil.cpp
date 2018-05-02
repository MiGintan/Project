// Fill out your copyright notice in the Description page of Project Settings.

#include "Eskil.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEskil::AEskil()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
	hammer->SetStaticMesh(hammerMesh);
	hammer->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));

	gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gun->SetStaticMesh(gunMesh);
	gun->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));
	
	hammerCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hammer Collision"));
	hammerCollision->SetupAttachment(hammer);
	hammerCollision->OnComponentBeginOverlap.AddDynamic(this, &AEskil::onOverlapBegin);

	radialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	radialForce->SetupAttachment(hammer);

	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile Spawn"));
	arrow->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));
}

void AEskil::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEskil::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEskil::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEskil::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEskil::LookUpAtRate);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &AEskil::teleport);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AEskil::attack);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AEskil::isUsingHammer);
}

// Called when the game starts or when spawned
void AEskil::BeginPlay()
{
	Super::BeginPlay();

	if (targetClass)
	{
		if (!target)
		{
			target = CreateWidget<UAim>(UGameplayStatics::GetPlayerController(GetWorld(), 0), targetClass);
			target->AddToViewport();
			target->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	gun->SetVisibility(false);
}

void AEskil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	usingGun();
}

void AEskil::teleport()
{
	if (canTeleport)
	{
		SetActorLocation(FVector(teleporterLocation.X, teleporterLocation.Y, teleporterLocation.Z + 42.0f), false, 0, ETeleportType::TeleportPhysics);
	}
}

void AEskil::attack()
{
	useGun = false;
	gun->SetVisibility(false);
	hammer->SetVisibility(true);

	if (attacking)
	{
		chainAttack = true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;

		switch (comboCounter)
		{
		case 0:
			comboCounter = 1;
			PlayAnimMontage(attackMontage1);
			break;
		}
	}
}

void AEskil::chainCombo()
{
	if (chainAttack)
	{
		chainAttack = false;
		switch (comboCounter)
		{
		case 1:
			PlayAnimMontage(attackMontage2);
			break;
		}
	}
}

void AEskil::comboReset()
{
	attacking = false;
	chainAttack = false;
	comboCounter = 0;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AEskil::usingGun()
{
	FCollisionQueryParams traceParams = FCollisionQueryParams();
	traceParams.bTraceComplex = false;
	traceParams.bTraceAsyncScene = true;
	traceParams.bReturnPhysicalMaterial = false;

	FHitResult hit(ForceInit);

	if (useGun)
	{
		bUseControllerRotationYaw = true;
		GetWorld()->LineTraceSingleByChannel(
			hit,
			getInitialLocation(),
			getInitialDirection() * aimDistance + getInitialLocation(),
			ECollisionChannel::ECC_Visibility,
			traceParams
		);

		DrawDebugLine(GetWorld(), getInitialLocation(), getInitialDirection() * aimDistance + getInitialLocation(), FColor::Black);

		if (hit.bBlockingHit)
		{
			if (hit.GetComponent()->GetName() == "Aim")
			{
				GLog->Log("Target Locked");
				targetLock = true;

				FVector2D screenPos;
				UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0), hit.Component->GetComponentLocation(), screenPos, true);
				target->SetVisibility(ESlateVisibility::Visible);
				target->SetPositionInViewport(screenPos);

				FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit.Actor->GetActorLocation());
				arrow->SetWorldRotation(rotation);
			}
			else
			{
				GLog->Log("No hit");
				target->SetVisibility(ESlateVisibility::Hidden);
				targetLock = false;
				arrow->SetRelativeRotation(FRotator(0, 35, 0));
			}
		}
	}
	else
	{
		bUseControllerRotationYaw = false;
		target->SetVisibility(ESlateVisibility::Hidden);
	}
}

FVector AEskil::getInitialLocation()
{
	return GetMesh()->GetComponentLocation() + GetActorForwardVector() * 80.0f + FVector(0, 0, 120.0f);
}

FVector AEskil::getInitialDirection()
{
	return FRotationMatrix(FRotator(GetControlRotation().Pitch, GetControlRotation().Yaw, GetMesh()->GetComponentRotation().Roll)).GetScaledAxis(EAxis::X);
}

void AEskil::isUsingHammer()
{
	if(!attacking)
	{
		useGun = true;
		gun->SetVisibility(true);
		hammer->SetVisibility(false);

		fireGun();
	}
}

void AEskil::setAttacking()
{
	attacking = true;
}

void AEskil::fireGun()
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	GetWorld()->SpawnActor<Aprojectile>(projectile, arrow->GetComponentLocation(), arrow->GetComponentRotation(), spawnParams);
}

void AEskil::onOverlapBegin(UPrimitiveComponent * overlapComp, AActor * otherActor, UPrimitiveComponent * otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (attacking)
	{
		UGameplayStatics::ApplyDamage(otherActor, 1.5f, NULL, this, NULL);
		FLatentActionInfo latentInfo;
		latentInfo.CallbackTarget = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayDynamicForceFeedback(0.3f, 0.1f, true, false, true, false, EDynamicForceFeedbackAction::Start, latentInfo);
		radialForce->FireImpulse();
	}
}

void AEskil::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEskil::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AEskil::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEskil::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
