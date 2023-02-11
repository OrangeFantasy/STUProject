// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class USTUWeaponComponent;
class USTUHealthComponent;
class USTUClimbingComponent;

UCLASS()
class UE5BUILD_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USTUClimbingComponent* ClimbingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Physics")
	bool bSimulatePhysics = false;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (EditCondition = "!bSimulatePhysics"))
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(1200.f, 2500.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

private:
	bool bIsMovingForward = false;

	bool bWantsToRun = false;

	bool bIsFlying = false;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsFlying() const { return bIsFlying; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsClimbing() const;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath();

private:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Fly();

	void OnStartRunning();

	void OnStopRunning();

	void OnHealthChanged(float Health);
	
	UFUNCTION(BlueprintCallable)
	void OnGroundLanded(const FHitResult& Hit);
};
