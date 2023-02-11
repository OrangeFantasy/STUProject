// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USTUWeaponFXComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*);

UCLASS()
class UE5BUILD_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

public:
	FOnClipEmptySignature OnClipNeedReload;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 2500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo = FAmmoData{ false, 15, 100, 0 };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

private:
	FAmmoData CurrentAmmo;

public:
	virtual void StartFire();

	virtual void StopFire();

	virtual void ReloadClip();

	void SpawnImpactFX(const FHitResult& Hit);

	bool TryToAddAmmo(int32 BulletsAmount);

	FWeaponUIData GetUIData() const { return UIData; }

	FAmmoData GetAmmoData() const { return CurrentAmmo; }

	bool IsAmmoFull() const;

	bool IsAmmoEmpty() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void MakeShot();

	virtual bool GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const;
	
	AController* GetPlayerController() const;

	bool GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const;

	FVector GetMuzzleWorldLocation() const;

	void MakeHit(FHitResult& OutHitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	void DecreaseAmmo();

	bool IsClipEmpty() const;

	UNiagaraComponent* SpawnMuzzleFX();

	void SpawnTraceFX(const FVector& TraceEnd) const;
};