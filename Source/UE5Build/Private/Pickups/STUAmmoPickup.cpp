// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUAmmoPickup::GivePickupTo(const APawn* Player)
{
	const USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player);
	if (HealthComponent && !HealthComponent->IsDeed())
	{
		USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Player);
		return WeaponComponent->TryToAddAmmo(WeaponType, BulletsAmount);
	}
	return false;
}
