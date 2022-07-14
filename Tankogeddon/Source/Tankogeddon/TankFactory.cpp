// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components\StaticMeshComponent.h"
#include "Components\BoxComponent.h"
#include "Components\ArrowComponent.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "Kismet\GameplayStatics.h"
#include "MapLoader.h"
#include "Particles\ParticleSystemComponent.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh->SetupAttachment(SceneComp);
	TempMesh->SetVisibility(false);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(BuildingMesh);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);


	TankSpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TankSpawnEffect"));
	TankSpawnEffect->SetupAttachment(BuildingMesh);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if (MapLoader)
	{
		MapLoader->SetIsActivated(false);
	}

	FTimerHandle SpawnTimer;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ATankFactory::SpawnTank, SpawnTankRate, true, 2.0f);
}

void ATankFactory::SpawnTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	newTank->SetPatrollingPoints(TankWayPoints);

	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);

	TankSpawnEffect->ActivateSystem();
}

void ATankFactory::Die()
{
	if (MapLoader)
	{
		MapLoader->SetIsActivated(true);
	}

	BuildingMesh->SetVisibility(false);
	TempMesh->SetVisibility(true);

	//Тут должна быть строка отключающая спаун танков, только не понял как написать

	//Destroy();
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s take Damage: %f,  Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

