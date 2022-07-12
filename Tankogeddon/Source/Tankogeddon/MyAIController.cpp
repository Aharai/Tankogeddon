// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "GameFramework\Pawn.h"

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPoints();
	for (FVector point : points)
	{
		PattrollingPath.Add(point + pawnLocation);
	}
	CurrentPattrolingIndex = 0;
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TankPawn->MoveForward(1);

	float RotationValue = GetRotationValue();
	TankPawn->RotateRight(RotationValue);

	Targeting();
}

float AMyAIController::GetRotationValue()
{
	FVector currentPoint = PattrollingPath[CurrentPattrolingIndex];
	FVector pawnLocation = TankPawn->GetActorLocation();
	if (FVector::Distance(currentPoint, pawnLocation) <= MovementAccurency)
	{
		CurrentPattrolingIndex++;
		if (CurrentPattrolingIndex >= PattrollingPath.Num())
		{
			CurrentPattrolingIndex = 0;
		}
	}

	FVector moveDirection = currentPoint - pawnLocation;
	moveDirection.Normalize();
	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), pawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float RightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float RotationValue = 0;
	if (forwardAngle > 5)
	{
		RotationValue = 1;
	}
	if (RightAngle > 90)
	{
		RotationValue = -RotationValue;
	}
	return RotationValue;
}

void AMyAIController::Targeting()
{
	if (CanFire())
		Fire();
	else
		RotateToPlayer();
}

void AMyAIController::RotateToPlayer()
{
	if (IsPlayerRange())
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}

bool AMyAIController::IsPlayerRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool AMyAIController::CanFire()
{
	//Не работает метод IsPlayerSeen(), все красиво рисуется, а танк не стреляет

	//if (!IsPlayerSeen())
	//{
	//	return false;
	//}
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));

	return aimAngle <= Accurency;
}

void AMyAIController::Fire()
{
	TankPawn->MoveForward(false);
	TankPawn->Fire();
}

bool AMyAIController::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetEyesPosition();

	FHitResult hitResult;

	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(TankPawn);
	traceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos,
		ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return hitResult.GetActor() == PlayerPawn;
		}
	}

	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;

}