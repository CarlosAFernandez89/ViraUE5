// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraGameplayAbility.h"
#include "VyraGameplayAbility_WithCharges.generated.h"

class UVyraAbilityTask_RechargeCharges;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_WithCharges : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UVyraGameplayAbility_WithCharges();

	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Charges")
	int32 MaxCharges;


};
