// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

UCLASS()
class TANKOGEDDON_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		class ATankPawn* TankPawn;

	UPROPERTY()
		class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
		TArray<FVector> PattrollingPath;

	int32 CurrentPattrolingIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
		float MovementAccurency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
		float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
		float Accurency = 10.0f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	float GetRotationValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerRange();
	bool CanFire();
	void Fire();

	bool IsPlayerSeen();
};