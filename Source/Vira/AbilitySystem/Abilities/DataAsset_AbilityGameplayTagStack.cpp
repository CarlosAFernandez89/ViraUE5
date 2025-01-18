// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset_AbilityGameplayTagStack.h"

TArray<FGameplayAbilityTagStackData> UDataAsset_AbilityGameplayTagStack::GetAllRelevantGameplayTags() const
{
	TArray<FGameplayAbilityTagStackData> AllTags;

	// Add the individual tags
	if (AbilityGameplayTagStack.AbilityLevel.GameplayTag.IsValid())
	{
		AllTags.Add(AbilityGameplayTagStack.AbilityLevel);
	}

	if (AbilityGameplayTagStack.AbilityCooldown.GameplayTag.IsValid())
	{
		AllTags.Add(AbilityGameplayTagStack.AbilityCooldown);
	}

	// Add the modifiers array
	for (FGameplayAbilityTagStackData Tag : AbilityGameplayTagStack.Modifiers)
	{
		if (Tag.GameplayTag.IsValid())
		{
			AllTags.Add(Tag);
		}
	}
	
	return AllTags;
}
