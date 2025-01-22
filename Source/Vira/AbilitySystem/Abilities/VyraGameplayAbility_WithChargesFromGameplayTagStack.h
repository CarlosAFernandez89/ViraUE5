// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraGameplayAbility.h"
#include "VyraGameplayAbility_WithChargesFromGameplayTagStack.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_WithChargesFromGameplayTagStack : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:

	UVyraGameplayAbility_WithChargesFromGameplayTagStack();
	
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Vyra|Ability|Charges")
	FGameplayTag ChargesTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Vyra|Ability|Charges")
	float ChargesConsumedPerActivation = 1.f;
};
