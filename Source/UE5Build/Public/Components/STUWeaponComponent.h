// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5BUILD_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;

	int32 CurrentWeaponIndex = 0;

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	bool bEquipAnimProgress = false;

	bool bReloadAnimProgress = false;

public:
	virtual void StartFire();

	virtual void StopFire();

	virtual void NextWeapon();

	void Reload();

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletsAmount);

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void EquipWeapon(int32 WeaponIndex);

	bool CanFire() const;

	bool CanEquip() const;

private:
	void SpawnWeapons();

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName SocketName);

	void InitAnimations();

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);

	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	void OnReloadClip(ASTUBaseWeapon* ClipReloadWeapon);

	bool CanReload() const;
};
