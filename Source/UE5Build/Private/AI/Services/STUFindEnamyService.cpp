// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUFindEnamyService.h"
#include "Components/STUAIPerceptionComponent.h"
#include "STUUtils.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USTUFindEnamyService::USTUFindEnamyService()
{
	NodeName = "Find Enamy";
	Interval = 1.0f;
}

void USTUFindEnamyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const USTUAIPerceptionComponent* PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(OwnerComp.GetAIOwner());
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnamyActorKey.SelectedKeyName, PerceptionComponent->GetClossestEmemy());
		}
	}
}

