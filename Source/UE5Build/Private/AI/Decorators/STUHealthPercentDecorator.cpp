// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/STUHealthPercentDecorator.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

#include "AIController.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
	NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		const USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(AIController->GetPawn());
		if (HealthComponent && !HealthComponent->IsDeed())
		{
			return HealthComponent->GetHealthPercent() < HealthPercentLimit;
		}
	}

	return false;
}

