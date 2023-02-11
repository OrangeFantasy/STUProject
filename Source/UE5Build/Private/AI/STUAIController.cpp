// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ASTUAICharacter* STUAICharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUAICharacter)
	{
		RunBehaviorTree(STUAICharacter->BehaviorTreeAsset);
	}
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	return GetBlackboardComponent() ? Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName)) : nullptr;
}

