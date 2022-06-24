// Fill out your copyright notice in the Description page of Project Settings.


#include "TankController.h"
#include "TankPawn.h"

void ATankController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankController::MoveRight);
}

void ATankController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	TankPawn = Cast<ATankPawn>(InPawn);
}

void ATankController::MoveForward(float Value)
{
	TankPawn->MoveForward(Value);
}

void ATankController::MoveRight(float Value)
{
	TankPawn->MoveRight(Value);
}
