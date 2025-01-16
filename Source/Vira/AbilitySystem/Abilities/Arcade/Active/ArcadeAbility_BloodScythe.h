// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/Arcade/VyraGameplayAbility_ArcadeBase.h"
#include "ArcadeAbility_BloodScythe.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_BloodScythe : public UVyraGameplayAbility_ArcadeBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "ArcadeAbility|BloodScythe")
	FGameplayAbilityTargetDataHandle PerformConeTrace(TArray<FHitResult>& OutHits);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BloodScythe")
	float ConeAngleDegrees = 45.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BloodScythe")
	float ConeLength = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bShowDebug = false;
};
