// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.hpp"

#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("The waepon number is not equal %i."), WeaponNum);

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (ASTUBaseWeapon* Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::StartFire()
{
	if (CanFire())
	{
		CurrentWeapon->StartFire();
	}
}

void USTUWeaponComponent::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void USTUWeaponComponent::NextWeapon()
{
	if (CanEquip())
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		EquipWeapon(CurrentWeaponIndex);
	}
}

void USTUWeaponComponent::Reload()
{
	if (CanReload())
	{
		bReloadAnimProgress = true;
		StopFire();
		PlayAnimMontage(CurrentReloadAnimMontage);
		CurrentWeapon->ReloadClip();
	}
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* const Character = Cast<ACharacter>(GetOwner());
	if (Character && GetWorld())
	{
		for (const FWeaponData& OneWeaponData : WeaponData)
		{
			ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
			if (Weapon)
			{
				Weapon->SetOwner(Character);
				Weapon->OnClipNeedReload.AddUObject(this, &USTUWeaponComponent::OnReloadClip);
				Weapons.Add(Weapon);

				AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
			}
			else
			{
				continue;
			}
		}
	}
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName SocketName)
{
	if (Weapon && SceneComponent)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
	}
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && CurrentWeaponIndex >= 0 && CurrentWeaponIndex < Weapons.Num())
	{
		if (CurrentWeapon)
		{
			// 将当前武器放回背后
			CurrentWeapon->StopFire();
			AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
		}

		// 装备新武器
		CurrentWeapon = Weapons[WeaponIndex];
		const FWeaponData* CurrentWeaponData = WeaponData.FindByPredicate(
			[&](const FWeaponData& Data) {
				return Data.WeaponClass == CurrentWeapon->GetClass();
			});
		CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
		bEquipAnimProgress = true;
		PlayAnimMontage(EquipAnimMontage);
	}
}

void USTUWeaponComponent::InitAnimations()
{
	USTUEquipFinishedAnimNotify* EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is not set."));
		checkNoEntry();
	}

	for (const FWeaponData& OneWeaponData : WeaponData)
	{
		USTUReloadFinishedAnimNotify* ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (ReloadFinishedNotify)
		{
			ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		}
		else
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is not set."));
			checkNoEntry();
		}
	}
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Character->PlayAnimMontage(Animation);
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetMesh() == MeshComponent)  // 筛选Mesh
	{
		bEquipAnimProgress = false;
	}
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetMesh() == MeshComponent)  // 筛选Mesh
	{
		bReloadAnimProgress = false;
	}
}

void USTUWeaponComponent::OnReloadClip(ASTUBaseWeapon* ClipReloadWeapon)
{
	if (CurrentWeapon == ClipReloadWeapon)
	{
		Reload();
	}
	else
	{
		for (const ASTUBaseWeapon* Weapon : Weapons)
		{
			if (Weapon == ClipReloadWeapon)
			{
				ClipReloadWeapon->ReloadClip();
			}
		}
	}
}


bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimProgress && !bReloadAnimProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimProgress && !bReloadAnimProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return !bEquipAnimProgress && !bReloadAnimProgress;
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType) const
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 BulletsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(BulletsAmount);
		}
	}
	return false;
}
