// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUProjectile.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(5.0);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	TraceNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("TraceNiagaraComponent");
	TraceNiagaraComponent->SetupAttachment(CollisionComponent);
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);
	check(MovementComponent);

	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectilsHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	
	SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectilsHit(UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetWorld())
	{
		MovementComponent->StopMovementImmediately();
		
		UGameplayStatics::ApplyRadialDamage(GetWorld(),
			DamageAmount,
			GetActorLocation(),
			DamageRadius,
			UDamageType::StaticClass(),
			{ GetOwner() },
			this,
			nullptr,
			bDoFullDamage,
			ECC_MAX);

		ProjetileHitObject.ExecuteIfBound(Hit);

		Destroy();
	}
}

AController* ASTUProjectile::GetController() const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

