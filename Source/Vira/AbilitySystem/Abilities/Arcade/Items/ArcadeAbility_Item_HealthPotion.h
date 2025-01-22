// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility_WithChargesFromGameplayTagStack.h"
#include "ArcadeAbility_Item_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_Item_HealthPotion : public UVyraGameplayAbility_WithChargesFromGameplayTagStack
{
	GENERATED_BODY()

	UArcadeAbility_Item_HealthPotion();

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	bool IsActorAtMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	void ApplyRecoveryEffect() const;
};
