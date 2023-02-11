// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"
#include "STUUtils.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* Pawn = Cast<ASTUBaseCharacter>(GetPawnOwner());

	return Pawn && Pawn->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

