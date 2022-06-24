// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ATankPawn::ATankPawn()
{

	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);

	Camera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float Value)
{
	TargetAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	TargetRightAxisValue = Value;
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CurrentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FVector movePosition = CurrentLocation + forwardVector * MoveSpeed * TargetAxisValue + (rightVector * MoveSpeed * TargetRightAxisValue);
	SetActorLocation(movePosition, true);
}

