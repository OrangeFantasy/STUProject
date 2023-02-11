// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/STUWeaponFXComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void USTUWeaponFXComponent::SpawnImpactFX(const FHitResult& HitResult)
{
	FImpactData ImpactData = DefaultImpactData;
	if (HitResult.PhysMaterial.IsValid())
	{
		const UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
		ImpactData.NiagaraEffect,
		HitResult.ImpactPoint, 
		HitResult.ImpactNormal.Rotation());

	UDecalComponent* DecalComponent =  UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
		ImpactData.DecalData.Material,
		ImpactData.DecalData.Size,
		HitResult.ImpactPoint,
		HitResult.ImpactNormal.Rotation());
	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}
}

void USTUWeaponFXComponent::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

UNiagaraComponent* USTUWeaponFXComponent::SpawnMuzzleFX(USkeletalMeshComponent* WeaponMesh, const FName& MuzzleSocketName)
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,
		WeaponMesh,
		MuzzleSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
}

