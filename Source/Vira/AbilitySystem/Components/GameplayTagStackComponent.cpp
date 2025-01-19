// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagStackComponent.h"

#include "Fonts/UnicodeBlockRange.h"


// Sets default values for this component's properties
UGameplayTagStackComponent::UGameplayTagStackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayTagStackComponent::InitializeNewStackTag(FGameplayTag Tag, const float CurrentCount, const float MaxCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::AddNewTagToStack"), ELogVerbosity::Warning);
		return;
	}

	if (TagStackContainer.ContainsTag(Tag))
	{
		FFrame::KismetExecutionMessage(TEXT("Tag already exists in TagStackContainer in UGameplayTagStackComponent::AddNewTagToStack"), ELogVerbosity::Warning);
		return;
	}

	TagStackContainer.AddStackForAbilitySystem(Tag, MaxCount == -1  ? CurrentCount : FMath::Min(CurrentCount, MaxCount));

	
	OnTagStackChanged.Broadcast(Tag, GetTagStackCount(Tag));
}

void UGameplayTagStackComponent::SetTagStack(const FGameplayTag Tag, const float NewStackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::SetTagStack"), ELogVerbosity::Warning);
		return;
	}

	TagStackContainer.ClearStack(Tag);
	const float ClampedStackCount = ClampToMaxStackCount(Tag, NewStackCount);
	TagStackContainer.AddStack(Tag, ClampedStackCount);

	OnTagStackChanged.Broadcast(Tag, GetTagStackCount(Tag));
}

void UGameplayTagStackComponent::AddToTagStack(const FGameplayTag Tag, const float CountToAdd)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::AddToTagStack"), ELogVerbosity::Warning);
		return;
	}

	const int32 ClampedCount = ClampToMaxStackCount(Tag, CountToAdd);
	TagStackContainer.AddStack(Tag, ClampedCount);

	OnTagStackChanged.Broadcast(Tag, GetTagStackCount(Tag));
}

void UGameplayTagStackComponent::RemoveTagStack(const FGameplayTag Tag, const float CountToRemove)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to UGameplayTagStackComponent::RemoveTagStack"), ELogVerbosity::Warning);
		return;
	}

	const float ClampedCount = ClampToMinStackCount(Tag, CountToRemove);
	TagStackContainer.RemoveStack(Tag, ClampedCount);

	OnTagStackChanged.Broadcast(Tag, GetTagStackCount(Tag));
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

float UGameplayTagStackComponent::ClampToMaxStackCount(FGameplayTag Tag, const float MaxCount) const
{
	
	const int32 TagStackIndex = FindGameplayTagStackIndexByGameplayTag(Tag);
	if (TagStackIndex == INDEX_NONE) return 0;
	
	if (GameplayTagStack[TagStackIndex].MaxCount <= -1.f)
	{
		// No stack limit, just keep adding.
		return MaxCount;
	}

	const float StackCurrentCount = TagStackContainer.GetStackCount(Tag);
	const float StackMaxCount = GameplayTagStack[TagStackIndex].MaxCount;

	float CountToAdd = FMath::Max(0, MaxCount);
	float AvailableCount = StackMaxCount - StackCurrentCount;
	if (AvailableCount < MaxCount)
	{
		CountToAdd = FMath::Max(0, AvailableCount);
	}

	return CountToAdd;
}

float UGameplayTagStackComponent::ClampToMinStackCount(FGameplayTag Tag, const float MinCount) const
{
	const int32 TagStackIndex = FindGameplayTagStackIndexByGameplayTag(Tag);
	if (TagStackIndex == INDEX_NONE) return 0;
	
	if (GameplayTagStack[TagStackIndex].MaxCount <= -1)
	{
		return MinCount;
	}

	const float StackCurrentCount = TagStackContainer.GetStackCount(Tag);
	const float StackMaxCount = GameplayTagStack[TagStackIndex].MaxCount;
	constexpr float StackMinCount = 0;

	float NewStackCount = StackCurrentCount - MinCount;

	NewStackCount = FMath::Clamp(NewStackCount, StackMinCount, StackMaxCount);

	const float ActualChange = StackCurrentCount - NewStackCount;

	return ActualChange;
}

