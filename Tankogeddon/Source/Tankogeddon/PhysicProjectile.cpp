
#include "PhysicProjectile.h"
#include "PhysicsComponent.h"
#include "Particles\ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet\KismetMathLibrary.h"
#include "DamageTaker.h"
#include "GameStruct.h"

APhysicProjectile::APhysicProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);

}

void APhysicProjectile::Start()
{
	MoveVector = GetActorForwardVector() * MoveSpeedPhysics;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, MaxTimeSimulation, TimeStep, 0.0f);

	if (bShowTrajectory)
	{
		for (FVector position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}
	TrajectoryPointIndex = 0;
	TrailEffect->ActivateSystem();

	Super::Start();
}

void APhysicProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("Not normalize: %s"), *CurrentMoveVector.ToString());
	CurrentMoveVector.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("Normalize: %s"), *CurrentMoveVector.ToString());

	FVector newLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);

	if (FVector::Distance(newLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MovementAccurency)
	{
		TrajectoryPointIndex++;
		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Explode();
			Destroy();
		}
		else
		{
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(newRotation);
		}
	}
}

