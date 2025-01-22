// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility_WithCharges.h"
#include "ArcadeAbility_Item_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UArcadeAbility_Item_HealthPotion : public UVyraGameplayAbility_WithCharges
{
	GENERATED_BODY()

	UArcadeAbility_Item_HealthPotion();

	protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	void ApplyRecoveryEffect() const;

	//This applies the cooldown in-between charges.
	void ApplySecondaryCooldown() const;
	//this checks for the cooldown in-between charges.
	bool CheckForSecondaryCooldown() const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Ability|SecondaryCooldown")
	TSubclassOf<UGameplayEffect> SecondaryCooldownClass;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Ability|SecondaryCooldown")
	FGameplayTag SecondaryCooldownGameplayTag;
};
