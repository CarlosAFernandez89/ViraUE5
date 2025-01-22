// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VyraAbilityTask_RechargeCharges.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeRecharged, float, CurrentCharges);
/**
 * 
 */
UCLASS()
class VIRA_API UVyraAbilityTask_RechargeCharges : public UAbilityTask
{
	GENERATED_BODY()

public:
	// Creates and starts the recharge task
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Start Recharge Charges", HideFromNodeList, HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UVyraAbilityTask_RechargeCharges* StartRechargeCharges(
		UGameplayAbility* OwningAbility,
		float CurrentCharges,
		float MaxCharges,
		float RechargeCooldown);

	// Delegate that notifies when a charge is recharged
	UPROPERTY(BlueprintAssignable)
	FOnChargeRecharged OnChargeRecharged;

protected:
	// Called when the task is activated
	virtual void Activate() override;

private:

	// Recharge logic
	void HandleRecharge();

	// Current number of charges
	float CurrentCharges;

	// Maximum number of charges
	float MaxCharges;

	// Cooldown for recharging one charge
	float RechargeCooldown;
};
