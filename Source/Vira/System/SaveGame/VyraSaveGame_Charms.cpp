// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraSaveGame_Charms.h"

#include "Kismet/GameplayStatics.h"
#include "Vira/Character/CharmManagerComponent.h"

UVyraSaveGame_Charms::UVyraSaveGame_Charms()
{
	SaveSlotName = TEXT("VyraSaveGame_Charms");
	UserIndex = 0;
}

void UVyraSaveGame_Charms::SaveCharms(const UCharmManagerComponent* CharmManagerComponent)
{
	if(CharmManagerComponent)
	{
		TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> OwnedCharmAbilities = CharmManagerComponent->GetOwnedCharms();
		if(!OwnedCharmAbilities.IsEmpty())
		{
			for (TSubclassOf<UVyraGameplayAbility_CharmBase> Charm : OwnedCharmAbilities)
			{
				AllCharms.OwnedCharms.Add(Charm);

				// Add this gameplay tag into the TMap if it doesnt already exist.
				// We don't add it if it exists already because it could have changed.
				for (auto TagStackData : Charm.GetDefaultObject()->CharmAbilityData)
				{
					// If we can't find the tag, we add it, else we do nothing
					FGameplayTag TagToAdd = FGameplayTag::RequestGameplayTag(TagStackData.GameplayTagStackTag.GetTagName());
					CharmAbilityData.FindOrAdd(TagToAdd.GetTagName(), TagStackData);
				}
			}
		}

		TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> EquippedCharmAbilities = CharmManagerComponent->GetOwnedCharms();
		if(!EquippedCharmAbilities.IsEmpty())
		{
			for (TSubclassOf<UVyraGameplayAbility_CharmBase> Charm : EquippedCharmAbilities)
			{
				AllCharms.EquippedCharms.Add(Charm);

				// Add this gameplay tag into the TMap if it doesnt already exist.
				// We don't add it if it exists already because it could have changed.
				for (auto TagStackData : Charm.GetDefaultObject()->CharmAbilityData)
				{
					// If we can't find the tag, we add it, else we do nothing
					FGameplayTag TagToAdd = FGameplayTag::RequestGameplayTag(TagStackData.GameplayTagStackTag.GetTagName());
					CharmAbilityData.FindOrAdd(TagToAdd.GetTagName(), TagStackData);
				}
			}
		}
	}
}

FSaveGame_Charms UVyraSaveGame_Charms::GetAllCharms() const
{
	return AllCharms;
}

TMap<FName, FCharmAbilityData> UVyraSaveGame_Charms::GetAllCharmTagStacks() const
{
	return CharmAbilityData;
}

void UVyraSaveGame_Charms::UpdateCharmAbilityDataTMap(const FCharmAbilityData& AbilityData)
{
	FGameplayTag TagToFind = FGameplayTag::RequestGameplayTag(AbilityData.GameplayTagStackTag.GetTagName());
	if(FCharmAbilityData* Data = CharmAbilityData.Find(TagToFind.GetTagName()))
	{
		Data->GameplayTagStackTagCount = AbilityData.GameplayTagStackTagCount;
		Data->GameplayTagStackTagMaxCount = AbilityData.GameplayTagStackTagMaxCount;
		FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;
		UGameplayStatics::AsyncSaveGameToSlot(this, SaveSlotName, UserIndex, SaveGameToSlotDelegate);
	}
}

void UVyraSaveGame_Charms::RemoveFromCharmAbilityDataTMap(const FCharmAbilityData& AbilityData)
{
	FGameplayTag TagToFind = FGameplayTag::RequestGameplayTag(AbilityData.GameplayTagStackTag.GetTagName());
	CharmAbilityData.Remove(TagToFind.GetTagName());
	FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;
	UGameplayStatics::AsyncSaveGameToSlot(this, SaveSlotName, UserIndex, SaveGameToSlotDelegate);
}
