﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "GameplayTagStack.generated.h"

struct FGameplayTagStackContainer;
struct FNetDeltaSerializeInfo;

/**
 * Represents one stack of a gameplay tag (tag + count)
 */
USTRUCT(BlueprintType)
struct FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGameplayTagStack()
	{}

	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{
	}

	FString GetDebugString() const;

public:

	FGameplayTag GetTag() const { return Tag; };
	float GetStackCount() const { return StackCount; };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Tag Stack")
	FName TagUserFacingName = FName();

private:
	friend FGameplayTagStackContainer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Tag Stack")
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Tag Stack")

	float StackCount = 0.f;
};

/** Container of gameplay tag stacks */
USTRUCT(BlueprintType)
struct FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FGameplayTagStackContainer()
	//	: Owner(nullptr)
	{
	}

public:
	
	void AddStackForAbilitySystem(FGameplayTag Tag, float StackCount);
	
	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	void AddStack(FGameplayTag Tag, float StackCount);

	void ClearStack(FGameplayTag Tag);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	void RemoveStack(FGameplayTag Tag, float StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	float GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	// Returns true if there is at least one stack of the specified tag
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
	}

	// Rebuild the TagToCountMap from the Stacks array
	void RebuildTagToCountMap()
	{
		TagToCountMap.Empty();
		for (const FGameplayTagStack& Stack : Stacks)
		{
			TagToCountMap.Add(Stack.Tag, Stack.StackCount);
		}
	}

public:

	
private:

	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;
	
	// Accelerated list of tag stacks for queries
	TMap<FGameplayTag, float> TagToCountMap;
};

template<>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};