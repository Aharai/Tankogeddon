// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"

class AProjectilePool;
UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

		DECLARE_EVENT_OneParam(ACannon, FChangeScore, float);

public:
	ACannon();

	void Fire();
	void FireSpecial();

	bool IsReadyToFire() { return bCanFire; };

	void Reload();

	void CreateProjectilePool();

	void SetupAmmo(int32 BoxAmmo);

	//Score
	float Score = 0.0f;

	UFUNCTION()
		void AddScore(float ScoreValue);

	FChangeScore ScoreChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		ECannonType CannonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float ReloadTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 Shells = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 BurstSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		float BurstInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		float FireRange = 100.0f;

	FTimerHandle ReloadTimer;
	FTimerHandle BurstTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<AProjectilePool> ProjectilePoolClass;

	UPROPERTY()
		AProjectilePool* ProjectilePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* AudioEffect;

private:
	bool bCanFire = true;

	int32 CurrentBurts = 0;

	void Burst();
};