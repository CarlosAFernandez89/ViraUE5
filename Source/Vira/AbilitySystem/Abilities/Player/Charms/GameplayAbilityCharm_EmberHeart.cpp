// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityCharm_EmberHeart.h"

UGameplayAbilityCharm_EmberHeart::UGameplayAbilityCharm_EmberHeart()
{
}

void UGameplayAbilityCharm_EmberHeart::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbilityCharm_EmberHeart::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityCharm_EmberHeart::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
