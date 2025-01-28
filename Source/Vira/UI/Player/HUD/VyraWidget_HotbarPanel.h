// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/UI/Player/VyraCommonActivatableWidget_WithASC.h"
#include "VyraWidget_HotbarPanel.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraWidget_HotbarPanel : public UCommonUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintNativeEvent, Category="HotbarPanel")
	TArray<int32> GetAvailableHotBarSlots() const;

	UFUNCTION(BlueprintNativeEvent, Category="HotbarPanel")
	void BindAbilityToSlot(const int32 SlotIndex, const FGameplayAbilitySpecHandle& AbilitySpecHandle);
};
