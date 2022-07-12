// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"
#include "Projectile.h"


void AProjectilePool::GetProjectile(FVector spawnLocation, FRotator spawnRotation)
{
	bHaveActiveProjectile = false;
	for (int32 i = 0; i < ProjectilePool.Num(); i++)
	{
		if (ProjectilePool[i]->bIsActivation == false)
		{
			ProjectilePool[i]->SetActorLocation(spawnLocation);
			ProjectilePool[i]->SetActorRotation(spawnRotation);
			ProjectilePool[i]->bIsActivation = true;
			bHaveActiveProjectile = true;
			ProjectilePool[i]->SetActorEnableCollision(true);
			ProjectilePool[i]->Start();
			return;
		}
	}
	if (bHaveActiveProjectile == false)
	{
		SpawnProjectile(spawnLocation, spawnRotation, true);
	}
}

void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

void AProjectilePool::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation, bool bIsActive /*= false */)
{
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

	if (projectile)
	{
		ProjectilePool.Add(projectile);
		projectile->bIsActivation = bIsActive;
		projectile->SetActorEnableCollision(bIsActive);

		if (bIsActive)
		{
			projectile->Start();
		}
	}
}

void AProjectilePool::InitializePool()
{
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnProjectile(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	}
}
