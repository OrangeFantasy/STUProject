// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STURifleWeapon.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

#include "Engine/World.h"
#include "NiagaraComponent.h"

void ASTURifleWeapon::StartFire()
{
	PlayMuzzleFX();
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimeHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimeHandle);
	SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
	FVector TraceStart, TraceEnd;
	if (GetWorld() && !IsAmmoEmpty() && GetTraceData(TraceStart, TraceEnd))
	{
		FHitResult HitResult;
		MakeHit(HitResult, TraceStart, TraceEnd);

		FVector TraceFXEnd = TraceEnd;
		if (HitResult.bBlockingHit)
		{
			TraceFXEnd = HitResult.Location;

			MakeDamage(HitResult);
			SpawnImpactFX(HitResult);
		}
		SpawnTraceFX(TraceFXEnd);

		DecreaseAmmo();
	}
	else
	{
		StopFire();
	}
}

bool ASTURifleWeapon::GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		OutTraceStart = ViewLocation;
		const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(BulletSpread));
		OutTraceEnd = OutTraceStart + ShootDirection * TraceMaxDistance;
		return true;
	}
	else
	{
		return false;
	}
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* DamageActor = HitResult.GetActor();
	if (DamageActor)
	{
		DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	}
}

void ASTURifleWeapon::PlayMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}

