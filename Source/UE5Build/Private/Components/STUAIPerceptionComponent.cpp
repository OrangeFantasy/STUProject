// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUHealthComponent.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(PerceptionLog, All, All);

AActor* USTUAIPerceptionComponent::GetClossestEmemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

	const AAIController* Controller = Cast<AAIController>(GetOwner());
	if (Controller && Controller->GetPawn())
	{
		float BestDistance = MAX_FLT;
		AActor* BestPawn = nullptr;
		for (AActor* PercieveActor : PercieveActors)
		{
			USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);
			if (HealthComponent && !HealthComponent->IsDeed())
			{
				const double CurrentDistance = (PercieveActor->GetActorLocation() - Controller->GetPawn()->GetActorLocation()).Size();
				if (CurrentDistance < BestDistance)
				{
					BestDistance = CurrentDistance;
					BestPawn = PercieveActor;
				}
			}
			UE_LOG(PerceptionLog, Display, TEXT("BestPawn"));
		}

		return BestPawn;
	}

	UE_LOG(PerceptionLog, Display, TEXT("nullptr"));
	return nullptr;
}
