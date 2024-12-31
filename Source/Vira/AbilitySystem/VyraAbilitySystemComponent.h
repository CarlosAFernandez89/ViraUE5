// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "VyraAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UVyraAbilitySystemComponent : public UGSCAbilitySystemComponent
{
	GENERATED_BODY()

	public:
	UVyraAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	TArray<UGameplayAbility*> GetAllActiveAbilitiesOfClass(TSubclassOf<UGameplayAbility> AbilityClass);

	template<typename T>
	TArray<T*> GetAllActiveAbilitiesOfClass_Typed();
};

template <typename T>
TArray<T*> UVyraAbilitySystemComponent::GetAllActiveAbilitiesOfClass_Typed()
{
	TArray<T*> Abilities;

	for (TArray<FGameplayAbilitySpec> AbilitySpecs = GetActivatableAbilities(); FGameplayAbilitySpec Spec : AbilitySpecs)
	{
		if (UGameplayAbility* SpecAbility = Spec.GetPrimaryInstance())
		{
			if (T* CastedAbility = Cast<T>(SpecAbility))
			{
				Abilities.Add(CastedAbility);
			}
		}
	}

	return Abilities;
}
