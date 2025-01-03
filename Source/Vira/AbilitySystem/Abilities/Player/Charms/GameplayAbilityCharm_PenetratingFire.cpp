// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityCharm_PenetratingFire.h"

UGameplayAbilityCharm_PenetratingFire::UGameplayAbilityCharm_PenetratingFire()
{
}

void UGameplayAbilityCharm_PenetratingFire::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                          const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbilityCharm_PenetratingFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityCharm_PenetratingFire::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
