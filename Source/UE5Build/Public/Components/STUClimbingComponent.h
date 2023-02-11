// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUClimbingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5BUILD_API USTUClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUClimbingComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float TraceDistanceFromMiddleForward = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float TraceDistanceFromHeadForward = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
	float ClimbingMaxSpeed = 210.0f;

	bool bIsClimbing = false;

private:
	bool bUseClimbingTrace = false;

	FVector ObstacleLocation;

	FVector ObstacleUpLocation;

	FVector ObstacleNormal;

	FVector ClimbingUpVector;

	//FVector ClimbingRightVector;

	FRotator ClimbingRotation;

	float PreviousMaxFlySpeed;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryToClimbing();

	FVector GetMovementUpVector() { return ClimbingUpVector; }

	//FVector GetRightVector() { return ClimbingRightVector; }

	bool IsClimbing() const { return bIsClimbing; }

protected:
	virtual void BeginPlay() override;

private:
	void StartClimbingTrace();

	bool TraceForwardFromMiddle();

	bool TraceForwardFromHead();

	void StopClimbingTrace();

	void GetMiddleFrowardTraceVector(FVector& OutTraceStart, FVector& OutTraceEnd, float TraceDistance);

	void GetHeadForwardTraceVector(FVector& OutTraceStart, FVector& OutTraceEnd, float TraceDistance);

	//void CalculateClimbingMovementVector();

	void SetClimbingParam();

};
