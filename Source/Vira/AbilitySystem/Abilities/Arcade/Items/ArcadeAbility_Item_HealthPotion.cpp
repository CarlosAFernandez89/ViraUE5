// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_Item_HealthPotion.h"

UArcadeAbility_Item_HealthPotion::UArcadeAbility_Item_HealthPotion()
{
	if (AbilityGameplayTagStack)
	{
		MaxCharges = AbilityGameplayTagStack->GetMaxCurrentTagCountByTag(FGameplayTag::RequestGameplayTag("GameplayTagStack.Arcade.Items.HealthPotion.Charges"));
	}
}

void UArcadeAbility_Item_HealthPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ApplyRecoveryEffect();
	ApplySecondaryCooldown();

	EndAbility(Handle, ActorInfo,ActivationInfo,true, false);
}

bool UArcadeAbility_Item_HealthPotion::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return CheckForSecondaryCooldown() && Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UArcadeAbility_Item_HealthPotion::ApplyRecoveryEffect() const
{
	if (TSubclassOf<UGameplayEffect> GameplayEffectClass = GetDamageGameplayEffectClass())
	{
		float AbilityLevel = GetGameplayTagStackCount(GetAbilityLevelTag());
		FGameplayEffectSpecHandle GESH = MakeOutgoingGameplayEffectSpec(GameplayEffectClass, AbilityLevel);
		if (GESH.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GESH);
		}
	}
}

void UArcadeAbility_Item_HealthPotion::ApplySecondaryCooldown() const
{
	if (SecondaryCooldownClass)
	{
		float AbilityLevel = GetGameplayTagStackCount(GetAbilityLevelTag());
		FGameplayEffectSpecHandle GESH = MakeOutgoingGameplayEffectSpec(SecondaryCooldownClass, AbilityLevel);
		if (GESH.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GESH);
		}
	}
}

bool UArcadeAbility_Item_HealthPotion::CheckForSecondaryCooldown() const
{
	if (SecondaryCooldownGameplayTag.IsValid())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get())
		{
			if (AbilitySystemComponent->HasMatchingGameplayTag(SecondaryCooldownGameplayTag))
			{
				return false;
			}
		}
	}
	return true;
}
