// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_WithCharges.h"

#include "AbilitySystemGlobals.h"


UVyraGameplayAbility_WithCharges::UVyraGameplayAbility_WithCharges(): MaxCharges(2)
{
}

bool UVyraGameplayAbility_WithCharges::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
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

	const FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(AbilityGameplayTagStack->GetCooldownGameplayTag().GetTagName());
	if (CooldownTag.IsValid())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			if (AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
			{
				
				int32 TotalTags = AbilitySystemComponent->GetTagCount(CooldownTag);
				
				if (TotalTags < MaxCharges)
				{
					return true;
				}
				
				if (OptionalRelevantTags)
				{
					const FGameplayTag& FailCooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;
					if (FailCooldownTag.IsValid())
					{
						OptionalRelevantTags->AddTag(FailCooldownTag);
					}

					// Let the caller know which tags were blocking
					FGameplayTagContainer RelevantTags;
					RelevantTags.AddTag(CooldownTag);
					OptionalRelevantTags->AppendMatchingTags(AbilitySystemComponent->GetOwnedGameplayTags(), RelevantTags);
				}

				return false;
			}
		}
	}
	return true;
}




