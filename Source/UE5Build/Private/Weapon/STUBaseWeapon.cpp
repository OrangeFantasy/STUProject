// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STUBaseWeapon.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");;
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	check(WeaponFXComponent);

	CurrentAmmo = DefaultAmmo;
	ReloadClip();
}

void ASTUBaseWeapon::StartFire()
{

}

void ASTUBaseWeapon::StopFire()
{

}

void ASTUBaseWeapon::MakeShot()
{

}

bool ASTUBaseWeapon::GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		OutTraceStart = ViewLocation;
		const FVector ShootDirection = ViewRotation.Vector();
		OutTraceEnd = OutTraceStart + ShootDirection * TraceMaxDistance;
		return true;
	}
	else
	{
		return false;
	}
}

AController* ASTUBaseWeapon::GetPlayerController() const
{
	const ACharacter* Player = Cast<ACharacter>(GetOwner());
	return Player ? Player->GetController<AController>() : nullptr;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const
{
	const AController* Controller = GetPlayerController();
	if (Controller)
	{
		Controller->GetPlayerViewPoint(OutViewLocation, OutViewRotation);
		return true;
	}
	else
	{
		return false;
	}
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	--CurrentAmmo.CurrentBullets;
	--CurrentAmmo.AmmoNum;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnClipNeedReload.Broadcast(this);
	}
}

void ASTUBaseWeapon::ReloadClip()
{
	if (CurrentAmmo.bInfinite)
	{
		CurrentAmmo.CurrentBullets = CurrentAmmo.ClipCapacity;
	}
	else
	{
		if (CurrentAmmo.AmmoNum > CurrentAmmo.ClipCapacity)
		{
			CurrentAmmo.CurrentBullets = CurrentAmmo.ClipCapacity;
		}
		else
		{
			CurrentAmmo.CurrentBullets = CurrentAmmo.AmmoNum;
		}
	}
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 BulletsAmount)
{
	if (!CurrentAmmo.bInfinite && !IsAmmoFull())
	{
		CurrentAmmo.AmmoNum = FMath::Min(CurrentAmmo.AmmoNum + BulletsAmount, DefaultAmmo.AmmoNum);
		if (CurrentAmmo.CurrentBullets != CurrentAmmo.ClipCapacity)
		{
			OnClipNeedReload.Broadcast(this);
		}

		return true;
	}
	return false;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.AmmoNum == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.bInfinite == false && CurrentAmmo.AmmoNum == DefaultAmmo.AmmoNum;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.CurrentBullets == 0;
}

void ASTUBaseWeapon::SpawnImpactFX(const FHitResult& Hit)
{
	WeaponFXComponent->SpawnImpactFX(Hit);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return WeaponFXComponent->SpawnMuzzleFX(WeaponMesh, MuzzleSocketName);
}

void ASTUBaseWeapon::SpawnTraceFX(const FVector& TraceEnd) const
{
	WeaponFXComponent->SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);
}
