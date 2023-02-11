// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUHealthPickup::GivePickupTo(const APawn* Player)
{
	USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Player);
	if (HealthComponent && !HealthComponent->IsDeed())
	{
		return HealthComponent->TryToAddHealth(HealthAmount);
	}
	return false;
}

