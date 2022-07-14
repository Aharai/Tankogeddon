// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "Scorable.h"
#include "MachinePawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API AMachinePawn : public APawn, public IDamageTaker, public IScorable
{
	GENERATED_BODY()

public:

	AMachinePawn();

	virtual void TakeDamage(FDamageData DamageData) override;

	void Fire();

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);

	//Scorable
	virtual float GetPoints() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scorable")
		float ScoreValue = 0;

	UFUNCTION()
		void ShowScore(float Value);

	float Score = 0.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<ACannon> EquippedCannonClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* DieEffect;

	UPROPERTY()
		ACannon* Cannon;

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);
};