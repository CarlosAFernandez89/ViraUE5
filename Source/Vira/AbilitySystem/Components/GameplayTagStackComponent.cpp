// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagStackComponent.h"


// Sets default values for this component's properties
UGameplayTagStackComponent::UGameplayTagStackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayTagStackComponent::SetTagStack(const FGameplayTag Tag, const int32 NewStackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::SetTagStack"), ELogVerbosity::Warning);
		return;
	}

	TagStackContainer.ClearStack(Tag);
	const int32 ClampedStackCount = ClampToMaxStackCount(Tag, NewStackCount);
	TagStackContainer.AddStack(Tag, ClampedStackCount);
}

void UGameplayTagStackComponent::AddToTagStack(const FGameplayTag Tag, const int32 CountToAdd)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::AddToTagStack"), ELogVerbosity::Warning);
		return;
	}

	const int32 ClampedCount = ClampToMaxStackCount(Tag, CountToAdd);
	TagStackContainer.AddStack(Tag, ClampedCount);

	OnTagStackChanged.Broadcast(Tag);
}

void UGameplayTagStackComponent::RemoveTagStack(const FGameplayTag Tag, const int32 CountToRemove)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::RemoveTagStack"), ELogVerbosity::Warning);
		return;
	}

	const int32 ClampedCount = ClampToMinStackCount(Tag, CountToRemove);
	TagStackContainer.RemoveStack(Tag, ClampedCount);

	OnTagStackChanged.Broadcast(Tag);
}

int32 UGameplayTagStackComponent::GetTagStackCount(const FGameplayTag Tag) const
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::GetTagStackCount"), ELogVerbosity::Warning);
		return -1;
	}
	
	return TagStackContainer.GetStackCount(Tag);
}

int32 UGameplayTagStackComponent::GetMaxTagStackCount(const FGameplayTag Tag) const
{
	for (auto AbilityData : GameplayTagStack)
	{
		if(AbilityData.GameplayTag == Tag)
		{
			return AbilityData.MaxCount;
		}
	}
	return -1;
}

bool UGameplayTagStackComponent::HasTagStack(FGameplayTag Tag) const
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to HasStatTag"), ELogVerbosity::Warning);
		return false;
	}
	
	return TagStackContainer.ContainsTag(Tag);
}

int32 UGameplayTagStackComponent::FindGameplayTagStackIndexByGameplayTag(const FGameplayTag& TagToFind) const
{
	return GameplayTagStack.IndexOfByPredicate([TagToFind](const FGameplayAbilityTagStackData& Data)
	{
		return Data.GameplayTag == TagToFind;
	});
}

int32 UGameplayTagStackComponent::ClampToMaxStackCount(FGameplayTag Tag, const int32 MaxCount) const
{
	
	const int32 TagStackIndex = FindGameplayTagStackIndexByGameplayTag(Tag);
	if (TagStackIndex == INDEX_NONE) return 0;
	
	if (GameplayTagStack[TagStackIndex].MaxCount <= -1)
	{
		// No stack limit, just keep adding.
		return MaxCount;
	}

	const int32 StackCurrentCount = TagStackContainer.GetStackCount(Tag);
	const int32 StackMaxCount = GameplayTagStack[TagStackIndex].MaxCount;

	int32 CountToAdd = FMath::Max(0, MaxCount);
	int32 AvailableCount = StackMaxCount - StackCurrentCount;
	if (AvailableCount < MaxCount)
	{
		CountToAdd = FMath::Max(0, AvailableCount);
	}

	return CountToAdd;
}

int32 UGameplayTagStackComponent::ClampToMinStackCount(FGameplayTag Tag, const int32 MinCount) const
{
	const int32 TagStackIndex = FindGameplayTagStackIndexByGameplayTag(Tag);
	if (TagStackIndex == INDEX_NONE) return 0;
	
	if (GameplayTagStack[TagStackIndex].MaxCount <= -1)
	{
		return MinCount;
	}

	const int32 StackCurrentCount = TagStackContainer.GetStackCount(Tag);
	const int32 StackMaxCount = GameplayTagStack[TagStackIndex].MaxCount;
	constexpr int32 StackMinCount = 0;

	int32 NewStackCount = StackCurrentCount - MinCount;

	NewStackCount = FMath::Clamp(NewStackCount, StackMinCount, StackMaxCount);

	const int32 ActualChange = StackCurrentCount - NewStackCount;

	return ActualChange;
}

