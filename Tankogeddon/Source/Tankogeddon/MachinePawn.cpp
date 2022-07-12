// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Components\BoxComponent.h"
#include "HealthComponent.h"
#include "Particles\ParticleSystemComponent.h"
#include "Cannon.h"

// Sets default values
AMachinePawn::AMachinePawn()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AMachinePawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMachinePawn::DamageTaked);

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	HitEffect->SetupAttachment(BodyMesh);

	DieEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DieEffect"));
	DieEffect->SetupAttachment(BodyMesh);
}

void AMachinePawn::TakeDamage(FDamageData DamageData)
{
	HitEffect->ActivateSystem();
	HealthComponent->TakeDamage(DamageData);
}

void AMachinePawn::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void AMachinePawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}

	if (Cannon)
	{
		Cannon->Destroy();
	}

	EquippedCannonClass = newCannonClass;

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(EquippedCannonClass, params);
	Cannon->SetOwner(this);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);

}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(EquippedCannonClass);
}

void AMachinePawn::Die()
{
	//Не смог найти решение т.к объект уничтожается, скорее всего нужен таймер
	// который я не смог написать((
	
	//DieEffect->ActivateSystem();

	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void AMachinePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s take Damage: %f,  Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}