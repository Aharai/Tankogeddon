// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "MachinePawn.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API ATurret : public AMachinePawn
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();

	bool IsPlayerSeen();

	UPROPERTY()
		class ATankPawn* TankPawn;

	UPROPERTY()
		class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 30.0f;

	const FString BodyMeshPath = "StaticMesh'/Game/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";

	const FString TurretMeshPath = "StaticMesh'/Game/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
};