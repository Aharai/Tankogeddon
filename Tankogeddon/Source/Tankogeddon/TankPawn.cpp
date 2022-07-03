// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);


	SpringArm = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CurrentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = CurrentLocation + forwardVector * MoveSpeed * TargetForwardAxisValue + (rightVector * MoveSpeed * TargetRightAxisValue);
	SetActorLocation(movePosition, true);

	CurrentRotateAxisValue = FMath::Lerp(CurrentRotateAxisValue, RotateRightAxisValue, InterpolationKey);
	float YawRotation = RotationSpeed * CurrentRotateAxisValue * DeltaSeconds;

	//UE_LOG(LogTemp, Warning, TEXT("CurrentRotateAxis Value: %f, RotaterRightAxisValue: %f"), CurrentRotateAxisValue, RotateRightAxisValue);

	FRotator CurrentRotation = GetActorRotation();

	YawRotation = CurrentRotation.Yaw + YawRotation;

	FRotator newRotation = FRotator(0.0f, YawRotation, 0.0f);
	SetActorRotation(newRotation);

	FVector MousePos = TankController->GetMousePosition();

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
	FRotator TurretRotatoin = TurretMesh->GetComponentRotation();

	targetRotation.Pitch = TurretRotatoin.Pitch;
	targetRotation.Roll = TurretRotatoin.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotatoin, targetRotation, RotateInterpolationKey));

}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f));

	TankController = Cast<ATankController>(GetController());

	SetupCannon(CannonClass);
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, params);

	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::WeaponChange()
{
	if (CannonClass)
	{
		SetupCannon(SecondCannonClass);
	}
	else
	{
		SetupCannon(CannonClass);
	}
}


