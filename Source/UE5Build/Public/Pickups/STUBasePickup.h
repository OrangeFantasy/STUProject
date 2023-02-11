// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class UE5BUILD_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	float RespawnTime = 5.0f;

private:
	FTimerHandle RespawnTimerHandle;

	float RotationYaw = 0.0f;

public:
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	virtual bool GivePickupTo(const APawn* Player);

	void PickupWasTaken();

	void Respawn();

	void GenerateRotationYaw();
};
