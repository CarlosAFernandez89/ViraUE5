// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAbilitySystemComponent.h"

UVyraAbilitySystemComponent::UVyraAbilitySystemComponent()
{
}

TArray<UGameplayAbility*> UVyraAbilitySystemComponent::GetAllActiveAbilitiesOfClass(
	TSubclassOf<UGameplayAbility> AbilityClass)
{
	TArray<UGameplayAbility*> Abilities;
	TArray<FGameplayAbilitySpec> AbilitySpecs = GetActivatableAbilities();
	
	for (FGameplayAbilitySpec Spec : AbilitySpecs)
	{
		if(UGameplayAbility* SpecAbility = Spec.GetPrimaryInstance())
		{
			if(SpecAbility->IsA(AbilityClass))
			{
				Abilities.Add(SpecAbility);
			}
		}
	}

	return Abilities;
}
