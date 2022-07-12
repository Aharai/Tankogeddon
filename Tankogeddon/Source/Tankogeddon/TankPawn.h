// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStruct.h"
#include "DamageTaker.h"
#include "MachinePawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API ATankPawn : public AMachinePawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void FireSpecial();
	void ChangeWeapon();

	virtual void Tick(float DeltaSeconds) override;

	//AI
	UFUNCTION()
		TArray<FVector> GetPatrollingPoints() { return PattrollingPoints; }

	UFUNCTION()
		float GetAccurency() { return MovementAccurency; }

	UFUNCTION()
		FVector GetTurretForwardVector();

	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = " Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = " Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Component")
		TSubclassOf<ACannon> SecondCannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotationSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotateInterpolationKey = 0.1f;

	UPROPERTY()
		class ATankController* TankController;

	//AI components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Components", Meta = (MakeEditWidget = true))
		TArray<FVector> PattrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Components")
		float MovementAccurency = 30.0f;

private:
	float TargetForwardAxisValue = 0.0f;
	float TargetRightAxisValue = 0.0f;

	float RotateRightAxisValue = 0.0f;
	float CurrentRotateAxisValue = 0.0f;
};