// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"


class UArrowComponent;
UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	void Fire();
	void FireSpecial();
	void Reload();

	void SetupAmmo(int32 BoxAmmo);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ReloadTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 Ammo = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BurstSize = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float BurstInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float FireRange = 1000.0f;

	FTimerHandle ReloadTimer;
	FTimerHandle BurstTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

private:
	bool bCanFire = true;

	int32 CurrentBurts = 0;

	void Burst();
};
