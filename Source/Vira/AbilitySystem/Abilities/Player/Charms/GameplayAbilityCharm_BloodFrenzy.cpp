﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityCharm_BloodFrenzy.h"

UGameplayAbilityCharm_BloodFrenzy::UGameplayAbilityCharm_BloodFrenzy()
{
}

void UGameplayAbilityCharm_BloodFrenzy::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbilityCharm_BloodFrenzy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityCharm_BloodFrenzy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
