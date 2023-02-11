// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"

#include "AIController.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
	Interval = 5.5f;
	RandomDeviation = 1.2f;
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		USTUAIWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(AIController->GetPawn());
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->NextWeapon();
		}
	}
}