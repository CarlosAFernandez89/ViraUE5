// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "VyraGameplayAbility_EnemyDeath.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_EnemyDeath : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UVyraGameplayAbility_EnemyDeath();
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnDeathEvent();
	
private:
	UPROPERTY()
	UGSCCoreComponent* GSCCoreComponent;
	
};
