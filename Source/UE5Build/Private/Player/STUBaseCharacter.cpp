// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUClimbingComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(Character_LOG, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponents");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

	ClimbingComponent = CreateDefaultSubobject<USTUClimbingComponent>("ClimbingComponent");
}

void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(ClimbingComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);
	check(GetCharacterMovement());
	check(GetMesh());
	check(ClimbingComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &ASTUBaseCharacter::Fly);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}

void ASTUBaseCharacter::MoveForward(float Value)
{
	bIsMovingForward = Value > 0.0f;

	if (Controller && Value != 0.0f)
	{
		FVector ForwardVector;
		if (IsFlying())
		{
			ForwardVector = CameraComponent->GetForwardVector();
		}
		else if (IsClimbing())
		{
			ForwardVector = ClimbingComponent->GetMovementUpVector();
		}
		else
		{
			ForwardVector = GetActorForwardVector();
		}
		AddMovementInput(ForwardVector, Value);
	}
}

void ASTUBaseCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASTUBaseCharacter::Jump()
{
	Super::Jump();

	ClimbingComponent->TryToClimbing();
}

void ASTUBaseCharacter::Fly()
{
	if (bIsFlying)
	{
		bIsFlying = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		bIsFlying = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

void ASTUBaseCharacter::OnStartRunning()
{
	bWantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
	bWantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
	return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::IsClimbing() const
{
	return ClimbingComponent->IsClimbing();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (!GetVelocity().IsZero())
	{
		FVector MovementVector = GetActorForwardVector();
		if (IsClimbing())
		{
			MovementVector = ClimbingComponent->GetMovementUpVector();
		}

		const FVector VelocityNormal = GetVelocity().GetSafeNormal();
		const double AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MovementVector, VelocityNormal)));
		const FVector CrossProduct = FVector::CrossProduct(MovementVector, VelocityNormal);

		return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
	}
	return 0.0f;
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnDeath()
{
	WeaponComponent->StopFire();

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetLifeSpan(LifeSpanOnDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	if (bSimulatePhysics)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(bSimulatePhysics);
	}
	else
	{
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
		PlayAnimMontage(DeathAnimMontage);
	}
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	bool bIsWalkable = GetCharacterMovement()->IsWalkable(Hit);
	if (bIsWalkable)
	{
		const float FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

		if (FallVelocityZ >= LandedDamageVelocity.X)
		{
			const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
			TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);

			UE_LOG(Character_LOG, Display, TEXT("Hard Ground Landed"));
		}
	}
}

