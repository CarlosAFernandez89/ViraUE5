// Fill out your copyright notice in the Description page of Project Settings.


#include "CharmManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "VyraPlayerStateCharacter.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/AbilitySystem/Abilities/Player/Combat/Charms/VyraGameplayAbility_CharmBase.h"
#include "Vira/System/SaveGame/VyraSaveGame_Charms.h"


// Sets default values for this component's properties
UCharmManagerComponent::UCharmManagerComponent()
{
}

UVyraAbilitySystemComponent* UCharmManagerComponent::GetVyraAbilitySystemComponent() const
{
	if(AActor* Owner = GetOwner())
	{
		if(UAbilitySystemComponent* ASC = Cast<AVyraPlayerStateCharacter>(Owner)->GetAbilitySystemComponent())
		{
			return Cast<UVyraAbilitySystemComponent>(ASC);
		}
	}
	
	return nullptr;
}

AVyraPlayerStateCharacter* UCharmManagerComponent::GetVyraPlayerStateCharacter() const
{
	if(AActor* Owner = GetOwner())
	{
		return Cast<AVyraPlayerStateCharacter>(Owner);
	}
	
	return nullptr;
}

bool UCharmManagerComponent::GiveCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> Ability_CharmBase)
{
	if(Ability_CharmBase)
	{
		OwnedCharmAbilities.Add(Ability_CharmBase);
		OnCharmsUpdated.Broadcast(Ability_CharmBase);
		GetVyraPlayerStateCharacter()->SaveCharms();
		return true;
	}
	return false;
}

bool UCharmManagerComponent::RemoveCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> Ability_CharmBase)
{
	if(OwnedCharmAbilities.Contains(Ability_CharmBase))
	{
		OwnedCharmAbilities.Remove(Ability_CharmBase);
		OnCharmsUpdated.Broadcast(Ability_CharmBase);
		GetVyraPlayerStateCharacter()->SaveCharms();
		return true;
	}

	return false;
}

bool UCharmManagerComponent::EquipCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> CharmToEquip)
{
	if(OwnedCharmAbilities.Contains(CharmToEquip))
	{
		if(UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
		{
			const FGameplayAbilitySpec AbilitySpec(CharmToEquip, 1, -1, this); // Level 1, InputID -1 (none), this (source object)
			FGameplayAbilitySpecHandle GrantedAbilityHandle = ASC->GiveAbility(AbilitySpec);
			if (GrantedAbilityHandle.IsValid()) // Check if the ability was granted successfully
			{
				EquippedCharmAbilities.Add(CharmToEquip);
				OnCharmsUpdated.Broadcast(CharmToEquip);
				return true;
			}
		}
	}
	return false;
}

void UCharmManagerComponent::UnEquipCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> CharmToUnEquip)
{
	if(EquippedCharmAbilities.Contains(CharmToUnEquip))
	{
		if(UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
		{
			TArray<UGameplayAbility*> AbilitiesOfClass = ASC->GetAllActiveAbilitiesOfClass(CharmToUnEquip);
			if(!AbilitiesOfClass.IsEmpty())
			{
				for (const UGameplayAbility* Ability : AbilitiesOfClass)
				{
					FGameplayAbilitySpecHandle AbilitySpecHandle = Ability->GetCurrentAbilitySpecHandle();
					if(AbilitySpecHandle.IsValid())
					{
						ASC->ClearAbility(AbilitySpecHandle);
					}
				}
				OnCharmsUpdated.Broadcast(CharmToUnEquip);
			}
		}
	}
}

void UCharmManagerComponent::SetCharmsOnLoad(const FSaveGame_Charms& AllCharms)
{
	EquippedCharmAbilities = AllCharms.EquippedCharms;
	OwnedCharmAbilities = AllCharms.OwnedCharms;
}

void UCharmManagerComponent::AddStackTagToAbility(TSubclassOf<UVyraGameplayAbility_CharmBase> CharmAbilityType,
	FGameplayTag Tag, int32 StackCount)
{
	if (UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
	{
		// Explicitly specify the template argument
		TArray<UVyraGameplayAbility_CharmBase*> Abilities = ASC->GetAllActiveAbilitiesOfClass_Typed<UVyraGameplayAbility_CharmBase>();

		// Process the abilities as needed
		for (UVyraGameplayAbility_CharmBase* Ability : Abilities)
		{
			if (Ability->IsA(CharmAbilityType)) // Check if the ability matches the specific type
			{
				// Add the stack tag or perform other operations
				Ability->AddStatTagStack(Tag, StackCount);
				if(UVyraSaveGame_Charms* SaveGame_Charms = GetVyraPlayerStateCharacter()->GetSaveGameCharms())
				{
					FCharmAbilityData NewData;
					FGameplayTag TagToAdd = FGameplayTag::RequestGameplayTag(Tag.GetTagName());
					NewData.GameplayTagStackTag = TagToAdd;
					NewData.GameplayTagStackTagCount = Ability->GetStatTagStackCount(TagToAdd);
					NewData.GameplayTagStackTagMaxCount = Ability->GetMaxStatTagStackCount(TagToAdd);
					SaveGame_Charms->UpdateCharmAbilityDataTMap(NewData);
				}
			}
		}
	}
}

void UCharmManagerComponent::RemoveStackTagFromAbility(TSubclassOf<UVyraGameplayAbility_CharmBase> CharmAbilityType,
	FGameplayTag Tag, int32 StackCount)
{
	if (UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
	{
		// Explicitly specify the template argument
		TArray<UVyraGameplayAbility_CharmBase*> Abilities = ASC->GetAllActiveAbilitiesOfClass_Typed<UVyraGameplayAbility_CharmBase>();

		// Process the abilities as needed
		for (UVyraGameplayAbility_CharmBase* Ability : Abilities)
		{
			if (Ability->IsA(CharmAbilityType)) // Check if the ability matches the specific type
			{
				// Add the stack tag or perform other operations
				Ability->RemoveStatTagStack(Tag, StackCount);
				if(UVyraSaveGame_Charms* SaveGame_Charms = GetVyraPlayerStateCharacter()->GetSaveGameCharms())
				{
					FCharmAbilityData NewData;
					FGameplayTag TagToAdd = FGameplayTag::RequestGameplayTag(Tag.GetTagName());
					NewData.GameplayTagStackTag = TagToAdd;
					NewData.GameplayTagStackTagCount = Ability->GetStatTagStackCount(TagToAdd);
					SaveGame_Charms->UpdateCharmAbilityDataTMap(NewData);
				}
			}
		}
	}
}


