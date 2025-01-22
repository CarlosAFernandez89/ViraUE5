// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_WithChargesFromGameplayTagStack.h"


UVyraGameplayAbility_WithChargesFromGameplayTagStack::UVyraGameplayAbility_WithChargesFromGameplayTagStack()
{
	
}

bool UVyraGameplayAbility_WithChargesFromGameplayTagStack::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ensure(ActorInfo))
	{
		return true;
	}
	
	if (!AbilityGameplayTagStack)
	{
		return true;
	}

	if (UVyraAbilitySystemComponent* VASC = GetVyraAbilitySystemComponent())
	{
		float Charges = VASC->GetGameplayTagStackCount(ChargesTag);

		if (Charges > 1.f)
		{
			return true;
		}

		return false;
	}

	return true;
}

void UVyraGameplayAbility_WithChargesFromGameplayTagStack::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if (UVyraAbilitySystemComponent* VASC = GetVyraAbilitySystemComponent())
	{
		float Charges = VASC->GetGameplayTagStackCount(ChargesTag);

		if (Charges > 1.f)
		{
			VASC->RemoveGameplayTagStack(ChargesTag, ChargesConsumedPerActivation);
		}
		
	}
	
}
