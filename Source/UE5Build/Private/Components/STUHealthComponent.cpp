// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

bool USTUHealthComponent::IsDeed() const
{
	return FMath::IsNearlyZero(Health);
}

bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.0f && !IsDeed() && GetWorld())
	{
		SetHealth(Health - Damage);

		GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);

		if (IsDeed())
		{
			OnDeath.Broadcast();
		}
		else if (bAutoHeal)
		{
			GetWorld()->GetTimerManager().SetTimer(HealTimeHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
		}
	}

	PlayCameraShake();
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (IsHealthFull())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);
	}
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (!IsDeed() && !IsHealthFull())
	{
		SetHealth(Health + HealthAmount);
		return true;
	}
	return false;
}

void USTUHealthComponent::PlayCameraShake() const
{
	if (!IsDeed())
	{
		const APawn* Player = Cast<APawn>(GetOwner());
		if (Player)
		{
			const APlayerController* Controller = Player->GetController<APlayerController>();
			if (Controller && Controller->PlayerCameraManager)
			{
				Controller->PlayerCameraManager->StartCameraShake(CameraShake);
			}
		}
	}
}

