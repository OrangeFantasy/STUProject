// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/STUFireService.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (Blackboard && AIController)
	{
		USTUAIWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(AIController->GetPawn());
		const AActor* Enamy = Cast<AActor>(Blackboard->GetValueAsObject(EnamyActorKey.SelectedKeyName));
		if (WeaponComponent && Enamy)
		{
			WeaponComponent->StartFire();
		}
	}
}

