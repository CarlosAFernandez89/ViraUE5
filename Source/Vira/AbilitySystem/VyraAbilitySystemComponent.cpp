// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAbilitySystemComponent.h"

#include "Abilities/DataAsset_AbilityGameplayTagStack.h"
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

void UVyraAbilitySystemComponent::ResetCooldownWithMatchingOwningTag(const FGameplayTagContainer CooldownTagContainer)
{
	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTagContainer);
	RemoveActiveEffects(Query);
}

void UVyraAbilitySystemComponent::ReduceCooldownWithMatchingOwningTag(const FGameplayTagContainer CooldownTagContainer,
	const float ReductionAmount)
{
	if (ReductionAmount <= 0.f)
	{
		return; // No need to reduce if the amount is zero or negative
	}

	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTagContainer);
	TArray<FActiveGameplayEffectHandle> MatchingEffects = GetActiveEffects(Query);
	for (const FActiveGameplayEffectHandle& Handle : MatchingEffects)
	{
		if (const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Handle))
		{
			// Get the remaining duration of the current cooldown effect
			const float RemainingDuration = ActiveEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
			// Calculate the new duration (ensure it's not negative)
			const float NewDuration = FMath::Max(RemainingDuration - ReductionAmount, 0.f);
			
			if (NewDuration > 0.f)
			{
				if (const TSubclassOf<UGameplayEffect> CooldownEffectClass = ActiveEffect->Spec.Def->GetClass())
				{
					// Create a new spec for the Gameplay Effect
					FGameplayEffectSpecHandle NewSpecHandle = MakeOutgoingSpec(CooldownEffectClass, ActiveEffect->Spec.GetLevel(), ActiveEffect->Spec.GetEffectContext());
					if (NewSpecHandle.IsValid())
					{
						//Set duration and GameplayTags from the old spec into the new one
						NewSpecHandle.Data->SetDuration(NewDuration, true);
						NewSpecHandle.Data->DynamicGrantedTags = ActiveEffect->Spec.DynamicGrantedTags;
						
						// Remove the current active effect
						RemoveActiveGameplayEffect(Handle);
					
						// Apply the new effect
						ApplyGameplayEffectSpecToSelf(*NewSpecHandle.Data);

						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
								FString::Format(TEXT("Reducing cooldown to : {0}"), { FString::SanitizeFloat(NewDuration) }));
						}
					}
				}
			}
			else
			{
				// Remaining cooldown would be less than 0. No need to re-apply.
				RemoveActiveGameplayEffect(Handle);
			}
		}
	}
}

void UVyraAbilitySystemComponent::AddGameplayTagStack(const FGameplayTag Tag, const float Count)
{
	GameplayTagStack->AddToTagStack(Tag, Count);
}

void UVyraAbilitySystemComponent::RemoveGameplayTagStack(const FGameplayTag Tag, const float Count)
{
	GameplayTagStack->RemoveTagStack(Tag, Count);
}

void UVyraAbilitySystemComponent::SetGameplayTagStack(const FGameplayTag Tag, const float Count)
{
	GameplayTagStack->SetTagStack(Tag, Count);
}

float UVyraAbilitySystemComponent::GetGameplayTagStackCount(const FGameplayTag Tag) const
{
	return GameplayTagStack->GetTagStackCount(Tag);
}

void UVyraAbilitySystemComponent::InitializeGameplayTagStack()
{
	for (const UDataAsset_AbilityGameplayTagStack* DataAsset : GameplayTagStack->RelevantGameplayTagStacks)
	{
		for (const FGameplayAbilityTagStackData GameplayTagStackData : DataAsset->GetAllRelevantGameplayTags())
		{
			GameplayTagStack->InitializeNewStackTag(
				GameplayTagStackData.GameplayTag,
				GameplayTagStackData.CurrentCount,
				GameplayTagStackData.MaxCount);
		}
	}
}
