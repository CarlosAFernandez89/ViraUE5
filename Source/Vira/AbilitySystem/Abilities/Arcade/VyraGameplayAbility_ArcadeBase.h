﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "VyraGameplayAbility_ArcadeBase.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_ArcadeBase : public UVyraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UVyraGameplayAbility_ArcadeBase();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "VyraGameplayAbility|Arcade")
	float GetAttackSpeed() const;
};
