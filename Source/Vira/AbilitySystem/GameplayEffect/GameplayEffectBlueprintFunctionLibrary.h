// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffectBlueprintFunctionLibrary.generated.h"

struct FVyraGameplayAbilityInfo;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class VIRA_API UGameplayEffectBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintPure, Category = "Vyra|GameplayEffect", meta = (WorldContext = "WorldContextObject"))
	static float GetModifierMagnitudeFromClass(const UObject* WorldContextObject, const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const int32 ModifierIndex = 0);

	UFUNCTION(BlueprintPure, Category = "Vyra|GameplayEffect", meta = (WorldContext = "WorldContextObject"))
	static float GetExecutionsMagnitudeFromClass(const UObject* WorldContextObject, const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const int32 ExecutionIndex = 0, const int32 ModifierIndex = 0);

	UFUNCTION(BlueprintPure, Category = "Vyra|GameplayEffect", meta = (WorldContext = "WorldContextObject"))
	static TArray<float> GetDamageValueForLevels(const UObject* WorldContextObject, const FVyraGameplayAbilityInfo& AbilityInfo);
};
