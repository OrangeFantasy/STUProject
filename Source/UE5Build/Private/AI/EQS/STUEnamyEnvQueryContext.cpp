// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/STUEnamyEnvQueryContext.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void USTUEnamyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
	if (Blackboard)
	{
		const AActor* ContextActor = Cast<AActor>(Blackboard->GetValueAsObject(EnamyActorKeyName));
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor);
	}
}

