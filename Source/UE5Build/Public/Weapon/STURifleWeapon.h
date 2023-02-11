// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;

UCLASS()
class UE5BUILD_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RifleShotParams")
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RifleShotParams")
	float BulletSpread = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DamageAmount = 10.0f;

private:
	FTimerHandle ShotTimeHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

public:
	virtual void StartFire() override;
	
	virtual void StopFire() override;

protected:
	virtual void MakeShot() override;

	virtual bool GetTraceData(FVector& OutTraceStart, FVector& OutTraceEnd) const override;
	
	void MakeDamage(const FHitResult& HitResult);

private:
	void PlayMuzzleFX();

	void SetMuzzleFXVisibility(bool Visible);
};
