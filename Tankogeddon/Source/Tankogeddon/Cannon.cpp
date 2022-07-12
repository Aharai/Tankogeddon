// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Components\SceneComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "ProjectilePool.h"
#include "Particles\ParticleSystemComponent.h"
#include "Components\AudioComponent.h"

// Sets default values
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* CannonSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CannonRoot"));
	RootComponent = CannonSceneComponent;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(CannonSceneComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonSceneComponent);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
}

void ACannon::Fire()
{
	if (!bCanFire || Shells == 0)
	{
		return;
	}

	bCanFire = false;
	Shells--;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));

		if (ProjectilePool)
		{
			ProjectilePool->GetProjectile(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		}
		else
		{
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			if (projectile)
			{
				projectile->SetOwner(this);
				projectile->Start();
			}
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

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Shells is: %d"), Shells));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, ReloadTime, false);
}

void ACannon::FireSpecial()
{
	if (!bCanFire || Shells == 0)
	{
		return;
	}

	bCanFire = false;
	Burst();
}

void ACannon::Reload()
{
	bCanFire = true;
}

void ACannon::SetupAmmo(int32 BoxAmmo)
{
	Shells += BoxAmmo;
}

void ACannon::CreateProjectilePool()
{
	if (ProjectilePoolClass)
		ProjectilePool = GetWorld()->SpawnActor<AProjectilePool>(ProjectilePoolClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();

	CreateProjectilePool();
}

void ACannon::Burst()
{
	if (CurrentBurts == BurstSize)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		bCanFire = true;
		CurrentBurts = 0;
		Shells--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Shells is: %d"), Shells));
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
