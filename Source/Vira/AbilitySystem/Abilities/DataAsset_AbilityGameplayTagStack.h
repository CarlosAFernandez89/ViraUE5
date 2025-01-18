// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Vira/AbilitySystem/Components/GameplayTagStackComponent.h"
#include "DataAsset_AbilityGameplayTagStack.generated.h"

USTRUCT(BlueprintType)
struct FAbilityGameplayTagStack
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAbilityTagStackData AbilityLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAbilityTagStackData AbilityCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayAbilityTagStackData> Modifiers;
};
/**
 * 
 */
UCLASS()
class VIRA_API UDataAsset_AbilityGameplayTagStack : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityGameplayTagStack AbilityGameplayTagStack;

	UFUNCTION(BlueprintCallable, Category = "AbilityGameplayTagStack")
	TArray<FGameplayAbilityTagStackData> GetAllRelevantGameplayTags() const;

	UFUNCTION(BlueprintCallable, Category = "AbilityGameplayTagStack")
	FGameplayTag GetAbilityLevelGameplayTag() const
	{
		return AbilityGameplayTagStack.AbilityLevel.GameplayTag;
	}
	UFUNCTION(BlueprintCallable, Category = "AbilityGameplayTagStack")
	FGameplayAbilityTagStackData GetAbilityLevelTagStackData() const
	{
		return AbilityGameplayTagStack.AbilityLevel;
	}
	
	UFUNCTION(BlueprintCallable, Category = "AbilityGameplayTagStack")
	FGameplayTag GetCooldownGameplayTag() const
	{
		return AbilityGameplayTagStack.AbilityCooldown.GameplayTag;
	}
};
