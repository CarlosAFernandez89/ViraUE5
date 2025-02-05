﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagStack.h"

#include "UObject/Stack.h"

//////////////////////////////////////////////////////////////////////
// FGameplayTagStack

FString FGameplayTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%f"), *Tag.ToString(), StackCount);
}

//////////////////////////////////////////////////////////////////////
// FGameplayTagStackContainer

void FGameplayTagStackContainer::AddStackForAbilitySystem(FGameplayTag Tag, float StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
	MarkItemDirty(NewStack);
	TagToCountMap.Add(Tag, StackCount);
}

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, float StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}
	if (StackCount > 0)
	{
		for (FGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const float NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				if(TagToCountMap.Contains(Tag))
				{
					TagToCountMap[Tag] = NewCount;
				}
				else
				{
					TagToCountMap.Add(Tag, NewCount);
				}
				MarkItemDirty(Stack);
				return;
			}
		}

		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
	}
}

void FGameplayTagStackContainer::ClearStack(FGameplayTag Tag)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}
	
	for (auto It = Stacks.CreateIterator(); It; ++It)
	{
		FGameplayTagStack& Stack = *It;
		if (Stack.Tag == Tag)
		{
			Stack.StackCount = 0;
			if(TagToCountMap.Contains(Tag))
			{
				TagToCountMap[Tag] = 0;
			}
			else
			{
				TagToCountMap.Add(Tag, 0);
			}
			MarkItemDirty(Stack);
			return;
		}
	}
	
}

void FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, float StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				float NewCount = Stack.StackCount - StackCount;
				NewCount = FMath::Clamp(NewCount, 0.1f, UE_BIG_NUMBER);
				Stack.StackCount = NewCount;
				if(TagToCountMap.Contains(Tag))
				{
					TagToCountMap[Tag] = NewCount;
				}
				else
				{
					TagToCountMap.Add(Tag, NewCount);
				}
				MarkItemDirty(Stack);
				
				return;
			}
		}
	}
}