// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"

#include "AIController.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		const USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(AIController->GetPawn());
		if (WeaponComponent)
		{
			return WeaponComponent->NeedAmmo(WeaponType);
		}
	}

	return false;
}
