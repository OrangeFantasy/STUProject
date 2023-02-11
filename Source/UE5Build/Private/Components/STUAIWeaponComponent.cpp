// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
	if (CanFire())
	{
		CurrentWeapon->IsAmmoEmpty() ? NextWeapon() : CurrentWeapon->StartFire();
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (CanEquip())
	{
		int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		while (NextWeaponIndex != CurrentWeaponIndex)
		{
			if (!Weapons[NextWeaponIndex]->IsAmmoEmpty()) {
				break;
			}
			NextWeaponIndex = (NextWeaponIndex + 1) % Weapons.Num();
		}

		if (CurrentWeaponIndex != NextWeaponIndex)
		{
			CurrentWeaponIndex = NextWeaponIndex;
			EquipWeapon(CurrentWeaponIndex);
		}
	}
}

