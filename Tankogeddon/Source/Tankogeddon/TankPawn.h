// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStruct.h"
#include "DamageTaker.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:

	ATankPawn();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void Fire();
	void FireSpecial();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);

	virtual void TakeDamage(FDamageData DamageData) override;

	void WeaponChange();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Component")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotateInterpolationKey = 0.1f;

	UPROPERTY()
	class ATankController* TankController;
	
private:
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;

	float RotateRightAxisValue = 0.0f;
	float CurrentRotateAxisValue = 0.0f;
};
