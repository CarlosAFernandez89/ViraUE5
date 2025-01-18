// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility_WithCastTime.h"
#include "ArcadeAbility_EmberBurst.generated.h"

class AVyraProjectileBase;
/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_EmberBurst : public UVyraGameplayAbility_WithCastTime
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "EmberBurst")
	void SpawnProjectilesInCone(const int32 NumProjectiles = 1, const float ConeHalfAngleDegrees = 45.f);

	UFUNCTION(BlueprintCallable, Category = "EmberBurst")
	int32 GetProjectileCount() const;
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vyra|Ability|Projectile")
	TSubclassOf<AVyraProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vyra|Ability|Projectile")
	TSubclassOf<UGameplayEffect> ProjectileDamageEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Vyra|Ability|Projectile")
	float ProjectileSpawnOffset = 35.f;
	
};
