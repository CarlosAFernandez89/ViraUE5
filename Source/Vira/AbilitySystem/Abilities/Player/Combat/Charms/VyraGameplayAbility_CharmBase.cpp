// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_CharmBase.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Vira/Character/CharmManagerComponent.h"
#include "Vira/System/SaveGame/VyraSaveGame_Charms.h"

UVyraGameplayAbility_CharmBase::UVyraGameplayAbility_CharmBase()
{
}

void UVyraGameplayAbility_CharmBase::SetStatTagStack(FGameplayTag Tag, const int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to SetStatTagStack"), ELogVerbosity::Warning);
		return;
	}
	
	TagStackContainer.ClearStack(Tag);
	TagStackContainer.AddStack(Tag, ClampToMaxStackCount(Tag, StackCount));
}

void UVyraGameplayAbility_CharmBase::AddStatTagStack(FGameplayTag Tag, const int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to SetStatTagStack"), ELogVerbosity::Warning);
		return;
	}

	int32 CountToAdd = ClampToMaxStackCount(Tag, StackCount);
	TagStackContainer.AddStack(Tag, CountToAdd);
	OnTagStacksChanged.Broadcast();
}

void UVyraGameplayAbility_CharmBase::RemoveStatTagStack(FGameplayTag Tag, const int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStatTagStack"), ELogVerbosity::Warning);
		return;
	}
	
	TagStackContainer.RemoveStack(Tag, ClampToMinStackCount(Tag, StackCount));
	OnTagStacksChanged.Broadcast();
}

int32 UVyraGameplayAbility_CharmBase::GetStatTagStackCount(FGameplayTag Tag) const
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to GetStatTagStackCount"), ELogVerbosity::Warning);
		return 0;
	}
	
	return TagStackContainer.GetStackCount(Tag);
}

int32 UVyraGameplayAbility_CharmBase::GetMaxStatTagStackCount(FGameplayTag Tag)
{
	for (auto AbilityData : CharmAbilityData)
	{
		if(AbilityData.GameplayTagStackTag == Tag)
		{
			return AbilityData.GameplayTagStackTagMaxCount;
		}
	}
	return -1;
}

bool UVyraGameplayAbility_CharmBase::HasStatTag(FGameplayTag Tag) const
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to HasStatTag"), ELogVerbosity::Warning);
		return false;
	}
	
	return TagStackContainer.ContainsTag(Tag);
}

int32 UVyraGameplayAbility_CharmBase::FindCharmAbilityDataIndexByGameplayTag(const FGameplayTag& TagToFind) const
{
	return CharmAbilityData.IndexOfByPredicate([TagToFind](const FCharmAbilityData& Data)
	{
		return Data.GameplayTagStackTag == TagToFind;
	});
}

int32 UVyraGameplayAbility_CharmBase::ClampToMaxStackCount(FGameplayTag Tag, int32 StackCount)
{
	
	const int32 Index = FindCharmAbilityDataIndexByGameplayTag(Tag);

	//IF we don't have a stack limit, we just add whatever is asked of us.
	if(CharmAbilityData[Index].GameplayTagStackTagMaxCount <= -1)
	{
		return StackCount;
	}
	

	int32 CurrentCount = TagStackContainer.GetStackCount(Tag);//60
	int32 MaxCount = CharmAbilityData[Index].GameplayTagStackTagMaxCount;//90

	//Don't want to add negatives.
	int32 CountToAdd = FMath::Max(0, StackCount);
	
	int32 AvailableCount = MaxCount - CurrentCount;
	if(AvailableCount < StackCount)
	{
		CountToAdd = FMath::Max(0, AvailableCount);;
	}

	return CountToAdd;
}

int32 UVyraGameplayAbility_CharmBase::ClampToMinStackCount(FGameplayTag Tag, int32 StackCountChange)
{
	const int32 Index = FindCharmAbilityDataIndexByGameplayTag(Tag);

	// If we don't have a stack limit, return the original StackCountChange (no clamping).
	if (CharmAbilityData[Index].GameplayTagStackTagMaxCount <= -1)
	{
		return StackCountChange;
	}

	int32 CurrentCount = TagStackContainer.GetStackCount(Tag);
	int32 MaxCount = CharmAbilityData[Index].GameplayTagStackTagMaxCount;
	int32 MinCount = 0; // Min count is always 0

	// Calculate the new count after the change.
	int32 NewCount = CurrentCount + StackCountChange;

	// Clamp to the min/max range.
	NewCount = FMath::Clamp(NewCount, MinCount, MaxCount);

	// Calculate the actual change that can be applied.
	int32 ActualChange = NewCount - CurrentCount;

	return ActualChange;
}

void UVyraGameplayAbility_CharmBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	OnTagStacksChanged.AddDynamic(this, &UVyraGameplayAbility_CharmBase::OnTagStacksChangedEvent);


	if(UVyraSaveGame_Charms* SaveGame_Charms = GetVyraPlayerStateCharacter()->GetSaveGameCharms())
	{
		for (FCharmAbilityData AbilityData : CharmAbilityData)
		{
			FGameplayTag TagToFind = FGameplayTag::RequestGameplayTag(AbilityData.GameplayTagStackTag.GetTagName());
			TMap<FName, FCharmAbilityData> CharmAbilityDataMap = SaveGame_Charms->GetAllCharmTagStacks();
			if(!CharmAbilityDataMap.IsEmpty())
			{
				if(FCharmAbilityData* Data = CharmAbilityDataMap.Find(TagToFind.GetTagName()))
				{
					SetStatTagStack(TagToFind, Data->GameplayTagStackTagCount);
				}
				else
				{
					SetStatTagStack(TagToFind, AbilityData.GameplayTagStackTagCount);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Could not find Tag: %s"), *TagToFind.ToString());
			}
		}
	}
	else
	{
		for (FCharmAbilityData AbilityData : CharmAbilityData)
		{
			FGameplayTag TagToAdd = FGameplayTag::RequestGameplayTag(AbilityData.GameplayTagStackTag.GetTagName());
			SetStatTagStack(TagToAdd, AbilityData.GameplayTagStackTagCount);
		}
	}
}

void UVyraGameplayAbility_CharmBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UVyraGameplayAbility_CharmBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVyraGameplayAbility_CharmBase::OnTagStacksChangedEvent()
{
	GetVyraPlayerStateCharacter()->SaveCharms();
}
