// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	if (GetWorld() && !IsAmmoEmpty())
	{
		FVector TraceStart, TraceEnd;
		if (GetTraceData(TraceStart, TraceEnd))
		{
			FHitResult HitResult;
			MakeHit(HitResult, TraceStart, TraceEnd);

			const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
			const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

			const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
			ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
			if (Projectile)
			{
				SpawnMuzzleFX();
				Projectile->ProjetileHitObject.BindUObject(this, &ASTUBaseWeapon::SpawnImpactFX);
				Projectile->SetShotDirection(Direction);
				Projectile->SetOwner(GetOwner());
				Projectile->FinishSpawning(SpawnTransform);
			}

			DecreaseAmmo();
		}
	}
}
