// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponFXComponent.generated.h"

class USkeletalMeshComponent;
class UNiagaraComponent;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5BUILD_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponFXComponent();

	void SpawnImpactFX(const FHitResult& HitResult);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	UNiagaraComponent* SpawnMuzzleFX(USkeletalMeshComponent* WeaponMesh, const FName& MuzzleSocketName);
		
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX | ImpactFX")
	FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX | ImpactFX")
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX | MuzzleFx")
	UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX | TraceFx")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX | TraceFX")
	FString TraceTargetName = "TraceTarget";
};
