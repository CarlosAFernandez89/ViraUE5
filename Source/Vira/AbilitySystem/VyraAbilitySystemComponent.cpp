// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAbilitySystemComponent.h"

#include "Components/GameplayTagStackComponent.h"

void UVyraAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameplayTagStack();
}

UVyraAbilitySystemComponent::UVyraAbilitySystemComponent()
{
	GameplayTagStack = CreateDefaultSubobject<UGameplayTagStackComponent>("GameplayTagStackComponent");
	GameplayTagStack->SetComponentTickEnabled(false);
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

void UVyraAbilitySystemComponent::AddGameplayTagStack(const FGameplayTag Tag, const int32 Count)
{
	GameplayTagStack->AddToTagStack(Tag, Count);
}

void UVyraAbilitySystemComponent::RemoveGameplayTagStack(const FGameplayTag Tag, const int32 Count)
{
	GameplayTagStack->RemoveTagStack(Tag, Count);
}

void UVyraAbilitySystemComponent::SetGameplayTagStack(const FGameplayTag Tag, const int32 Count)
{
	GameplayTagStack->SetTagStack(Tag, Count);
}

int32 UVyraAbilitySystemComponent::GetGameplayTagStackCount(const FGameplayTag Tag) const
{
	return GameplayTagStack->GetTagStackCount(Tag);
}

void UVyraAbilitySystemComponent::InitializeGameplayTagStack()
{
	for (const FGameplayAbilityTagStackData Element : GameplayTagStack->GameplayTagStack)
	{
		AddGameplayTagStack(Element.GameplayTag, Element.CurrentCount);
	}
}
