// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "ArcadeAbility_EmberBurst.generated.h"

class AVyraProjectileBase;
/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_EmberBurst : public UVyraGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "EmberBurst")
	void SpawnProjectilesInCone(const int32 NumProjectiles = 1, const float ConeHalfAngleDegrees = 45.f);

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AVyraProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Projectile")
	TSubclassOf<UGameplayEffect> ProjectileDamageEffect;
};
