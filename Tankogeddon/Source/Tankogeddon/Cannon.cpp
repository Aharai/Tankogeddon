// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* CannonSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = CannonSceneComponent;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::Fire()
{
	if (!bCanFire || Ammo == 0)
	{
		return;
	}

	bCanFire = false;
	Ammo--;

	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));

		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace")));

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		FVector StartTrace = ProjectileSpawnPoint->GetComponentLocation();
		FVector EndTrace = StartTrace + ProjectileSpawnPoint->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), StartTrace, hitResult.Location, FColor::Red, false, 0.5f, 0, 10);
			if (hitResult.GetActor())
			{
				hitResult.GetActor()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Purple, false, 0.5f, 0, 10);
		}


	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Ammo is: %d"), Ammo));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, ReloadTime, false);
}

void ACannon::FireSpecial()
{
	if (!bCanFire)
	{
		return;
	}

	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, ReloadTime, false);

	Burst();
}

void ACannon::Reload()
{
	bCanFire = true;
}

void ACannon::SetupAmmo(int32 BoxAmmo)
{
	Ammo += BoxAmmo;
}

void ACannon::Burst()
{
	if (CurrentBurts == BurstSize)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		bCanFire = true;
		CurrentBurts = 0;
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Shells is: %d"), Ammo));
		return;
	}

	CurrentBurts++;


	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("Fire projectile")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("Fire trace")));
	}

	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::Burst, BurstInterval, false);
}

