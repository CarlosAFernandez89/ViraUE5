// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/Player/Combat/Charms/VyraGameplayAbility_CharmBase.h"
#include "GameplayAbilityCharm_ExecutionersStrike.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbilityCharm_ExecutionersStrike : public UVyraGameplayAbility_CharmBase
{
	GENERATED_BODY()

public:
	UGameplayAbilityCharm_ExecutionersStrike();
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GameplayAbility|Charms")
	TSubclassOf<UGameplayEffect> EffectClass;
};
