// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWidget_HotbarPanel.h"

TArray<int32> UVyraWidget_HotbarPanel::GetAvailableHotBarSlots_Implementation() const
{
	return TArray<int32>();
}

void UVyraWidget_HotbarPanel::BindAbilityToSlot_Implementation(const int32 SlotIndex,
	const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
}
