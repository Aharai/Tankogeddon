// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DamageTaker.h"
#include "GameStruct.h"
#include "MachinePawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API AMachinePawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:

	AMachinePawn();

	virtual void TakeDamage(FDamageData DamageData) override;

	void Fire();

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);

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

	FTimerHandle DieTimer;

	UPROPERTY()
		ACannon* Cannon;

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);
};