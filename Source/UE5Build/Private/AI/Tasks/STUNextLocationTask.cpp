// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STUNextLocationTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
	NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (Blackboard && AIController && AIController->GetPawn())
	{
		AActor* AIPawn = AIController->GetPawn();
		const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIPawn);
		if (NavSys)
		{
			FVector Location = AIPawn->GetActorLocation();
			if (!bSelfAsCenterActor)
			{
				AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
				if (CenterActor)
				{
					Location = CenterActor->GetActorLocation();
				}
			}

			FNavLocation NavResultLocation;
			bool bFound = NavSys->GetRandomReachablePointInRadius(Location, NavigationRandomRadius, NavResultLocation);
			if (bFound)
			{
				Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavResultLocation.Location);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
