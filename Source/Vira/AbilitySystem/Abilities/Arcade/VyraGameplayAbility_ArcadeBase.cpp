// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_ArcadeBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"

UVyraGameplayAbility_ArcadeBase::UVyraGameplayAbility_ArcadeBase()
{
}

void UVyraGameplayAbility_ArcadeBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	// We can modify the starting level of the ability be modifying this
	//FGameplayAbilitySpec NewAbilitySpec(Spec.Ability, Spec.Level);

	Super::OnGiveAbility(ActorInfo, Spec);
}

void UVyraGameplayAbility_ArcadeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// If the ability cannot be committed, end it here
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
}

void UVyraGameplayAbility_ArcadeBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

float UVyraGameplayAbility_ArcadeBase::GetAttackSpeed() const
{
	bool bFoundAttribute = false;
	const float AttackSpeed = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UCombatAttributeSet::GetAttackSpeedAttribute(), bFoundAttribute);

	return  bFoundAttribute ? AttackSpeed : 1.f;
}
