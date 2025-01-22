// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_Item_HealthPotion.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Attributes/GSCAttributeSet.h"

UArcadeAbility_Item_HealthPotion::UArcadeAbility_Item_HealthPotion()
{
}

bool UArcadeAbility_Item_HealthPotion::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return !IsActorAtMaxHealth() && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

bool UArcadeAbility_Item_HealthPotion::IsActorAtMaxHealth() const
{
	bool bGotHealth = false;
	const float Health = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UGSCAttributeSet::GetHealthAttribute(), bGotHealth);
	bool bGotMaxHealth = false;
	const float MaxHealth = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UGSCAttributeSet::GetMaxHealthAttribute(), bGotMaxHealth);

	if (bGotHealth && bGotMaxHealth)
	{
		if (Health >= MaxHealth)
		{
			return true;
		}

		return false;
	}

	return true;
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
