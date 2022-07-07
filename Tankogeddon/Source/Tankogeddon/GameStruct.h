// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use Projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use Trace"),
	FireArrow = 2 UMETA(DisplayName = "Use Arrow")
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

		UPROPERTY()
		float DamageValue;

	UPROPERTY()
		AActor* Instigator;

	UPROPERTY()
		AActor* DamageMaker;
};

UCLASS()
class TANKOGEDDON_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};
