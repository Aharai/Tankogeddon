// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "TankController.h"
#include "Kismet\KismetMathLibrary.h"
#include "Cannon.h"
#include "Components\ArrowComponent.h"
#include "HealthComponent.h"


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float Value)
{
	TargetForwardAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	TargetRightAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}


void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::ChangeWeapon()
{
	//Change weapon
	TSubclassOf<ACannon> cachedCannon = EquippedCannonClass;
	EquippedCannonClass = SecondCannonClass;
	SecondCannonClass = cachedCannon;

	//EquipWeapon
	SetupCannon(EquippedCannonClass);
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//tank movement
	FVector CurrentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = CurrentLocation + (forwardVector * MoveSpeed * TargetForwardAxisValue) + (rightVector * MoveSpeed * TargetRightAxisValue);
	SetActorLocation(movePosition, true);

	//Tank rotation
	CurrentRotateAxisValue = FMath::Lerp(CurrentRotateAxisValue, RotateRightAxisValue, InterpolationKey);
	float YawRotation = RotationSpeed * CurrentRotateAxisValue * DeltaSeconds;

	//UE_LOG(LogTemp, Warning, TEXT("CurrentRotateAxis Value: %f, RotateRightAxisValue: %f"), CurrentRotateAxisValue, RotateRightAxisValue);

	FRotator CurrentRotation = GetActorRotation();

	YawRotation = CurrentRotation.Yaw + YawRotation;
	//YawRotation += CurrentRotation.Yaw;

	FRotator newRotation = FRotator(0.0f, YawRotation, 0.0f);
	SetActorRotation(newRotation);

	//Turret Rotation
	if (TankController)
	{
		FVector MousePos = TankController->GetMousePosition();
		RotateTurretTo(MousePos);
	}

}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, targetRotation, RotateInterpolationKey));
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f));

	TankController = Cast<ATankController>(GetController());

}