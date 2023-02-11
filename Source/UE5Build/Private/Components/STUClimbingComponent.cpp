// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUClimbingComponent.h"
#include "STUUtils.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ClimbingComponentLog, All, All);

USTUClimbingComponent::USTUClimbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void USTUClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTUClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUseClimbingTrace)
	{
		StartClimbingTrace();
	}
}

void USTUClimbingComponent::TryToClimbing()
{
	bUseClimbingTrace = true;
}

void USTUClimbingComponent::StartClimbingTrace()
{
	bool bMiddleForwardHasObstacle = TraceForwardFromMiddle();
	if (bMiddleForwardHasObstacle)
	{
		bool bHeadForwardHasObstacle = TraceForwardFromHead();
		if (!bHeadForwardHasObstacle)
		{
			StopClimbingTrace();
		}
	}
	else
	{
		StopClimbingTrace();
	}
}

bool USTUClimbingComponent::TraceForwardFromMiddle()
{
	if (GetWorld())
	{
		FVector TraceStart, TraceEnd;
		GetMiddleFrowardTraceVector(TraceStart, TraceEnd, TraceDistanceFromMiddleForward);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
		if (HitResult.bBlockingHit && (HitResult.Normal - FVector(0, 0, 1)).Length() > 1)
		{
			ObstacleLocation = HitResult.Location;
			ObstacleNormal = HitResult.Normal;
			ClimbingRotation = UKismetMathLibrary::MakeRotFromX(-HitResult.Normal);

			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Yellow, false, 2.0f, 0, 2.0f);
			DrawDebugSphere(GetWorld(), HitResult.Location, 15.0f, 0, FColor::Yellow, false, 2.0f, 0, 2.0f);

			return true;
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 2.0f, 0, 2.0f);
		}
	}
	return false;
}

bool USTUClimbingComponent::TraceForwardFromHead()
{
	if (GetWorld())
	{
		FVector TraceStart, TraceEnd;
		GetHeadForwardTraceVector(TraceStart, TraceEnd, TraceDistanceFromHeadForward);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
		if (HitResult.bBlockingHit)
		{
			ObstacleUpLocation = HitResult.Location;
			ClimbingUpVector = (ObstacleUpLocation - ObstacleLocation).GetSafeNormal();

			//CalculateClimbingMovementVector();

			if (!bIsClimbing)
			{
				SetClimbingParam();
			}

			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Green, false, 2.0f, 0, 2.0f);
			DrawDebugSphere(GetWorld(), HitResult.Location, 5.0f, 0, FColor::Green, false, 2.0f, 0, 2.0f);

			return true;
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 2.0f);
		}
	}
	return false;
}

void USTUClimbingComponent::GetMiddleFrowardTraceVector(FVector& OutTraceStart, FVector& OutTraceEnd, float TraceDistance)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		OutTraceStart = Character->GetActorLocation();
		OutTraceEnd = OutTraceStart + Character->GetActorForwardVector() * TraceDistance;
	}
}

void USTUClimbingComponent::GetHeadForwardTraceVector(FVector& OutTraceStart, FVector& OutTraceEnd, float TraceDistance)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		OutTraceStart = Character->GetActorLocation() + FVector(0, 0, CapsuleHalfHeight);
		OutTraceEnd = OutTraceStart + Character->GetActorForwardVector() * TraceDistance;
	}
}

//void USTUClimbingComponent::CalculateClimbingMovementVector()
//{
//	ClimbingUpVector = (ObstacleUpLocation - ObstacleLocation).GetSafeNormal();
//	ClimbingRightVector = FVector::CrossProduct(ObstacleNormal, ClimbingUpVector);
//}

void USTUClimbingComponent::SetClimbingParam()
{
	bIsClimbing = true;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Character->bUseControllerRotationYaw = false;
		Character->SetActorRotation(FQuat::MakeFromRotator(ClimbingRotation));

		UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Character->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		PreviousMaxFlySpeed = MovementComponent->MaxFlySpeed;
		MovementComponent->MaxFlySpeed = ClimbingMaxSpeed;

		UE_LOG(ClimbingComponentLog, Display, TEXT("MaxFlySpeed = %f, ClimbingMaxSpeed = %f"), PreviousMaxFlySpeed, ClimbingMaxSpeed);
	}
}

void USTUClimbingComponent::StopClimbingTrace()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Character->bUseControllerRotationYaw = true;

		UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Character->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		if (MovementComponent && !MovementComponent->IsFalling())
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			bUseClimbingTrace = false;

			if (bIsClimbing)
			{
				bIsClimbing = false;

				FVector NewLocation = Character->GetActorLocation() + (Character->GetActorForwardVector() + FVector(0, 0, 1)) * 150;
				Character->SetActorLocation(NewLocation);

				MovementComponent->MaxFlySpeed = PreviousMaxFlySpeed;
				UE_LOG(ClimbingComponentLog, Display, TEXT("MaxFlySpeed = %f, ClimbingMaxSpeed = %f"), PreviousMaxFlySpeed, ClimbingMaxSpeed);
			}
		}
	}
}

