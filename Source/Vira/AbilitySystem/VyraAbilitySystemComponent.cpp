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

bool UVyraAbilitySystemComponent::HasAbilityOfClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
		{
			return true; // Ability already exists
		}
	}
	return false;
}

bool UVyraAbilitySystemComponent::GiveUniqueAbility(FGameplayAbilitySpecHandle& SpecHandle,TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID)
{
	if (!HasAbilityOfClass(AbilityClass))
	{
		FGameplayAbilitySpec Spec(AbilityClass, Level, InputID);
		SpecHandle = GiveAbility(Spec);
		return true;
	}

	return false;
}