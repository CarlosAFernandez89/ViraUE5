// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility_WithCastTime.h"
#include "ArcadeAbility_InfernalDash.generated.h"


class AInfernalDash_TrailDamageActor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportEnd, FVector, EndLocation);
/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_InfernalDash : public UVyraGameplayAbility_WithCastTime
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "InfernalDash")
	void PerformTeleport();

	void SpawnFireTrail(const FVector& StartLocation, const FVector& EndLocation);

public:

	UPROPERTY(BlueprintAssignable, Category = "InfernalDash")
	FOnTeleportEnd OnTeleportEnd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash")
	float TeleportDistance = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	bool bSpawnFireTrail = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	TSubclassOf<AInfernalDash_TrailDamageActor> TrailDamageActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	float TrailDamageSpawnInterval = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	float TrailDamageLifeSpan = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	TSubclassOf<UGameplayEffect> TrailDamageEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernalDash|Trail")
	float TrailDamage_DamageInterval = 0.5f;
};
