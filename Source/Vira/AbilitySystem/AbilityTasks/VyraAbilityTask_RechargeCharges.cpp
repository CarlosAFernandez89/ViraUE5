// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAbilityTask_RechargeCharges.h"

UVyraAbilityTask_RechargeCharges* UVyraAbilityTask_RechargeCharges::StartRechargeCharges(
	UGameplayAbility* OwningAbility,float CurrentCharges, float MaxCharges, float RechargeCooldown)
{
	// Create the ability task
	UVyraAbilityTask_RechargeCharges* Task = NewAbilityTask<UVyraAbilityTask_RechargeCharges>(OwningAbility);

	// Initialize variables
	Task->MaxCharges = MaxCharges;
	Task->CurrentCharges = CurrentCharges;
	Task->RechargeCooldown = RechargeCooldown;

	return Task;
}

void UVyraAbilityTask_RechargeCharges::Activate()
{
	// Start the recharge timer if charges are not full
	if (CurrentCharges < MaxCharges)
	{
		if (UWorld* CurrentWorld = GetWorld())
		{
			FTimerHandle TimerHandle;
			CurrentWorld->GetTimerManager().SetTimer(TimerHandle, this, &UVyraAbilityTask_RechargeCharges::HandleRecharge, RechargeCooldown, false);
		}
	}
}


void UVyraAbilityTask_RechargeCharges::HandleRecharge()
{
	// Increment charges
	if (CurrentCharges < MaxCharges)
	{
		CurrentCharges = FMath::Clamp(CurrentCharges + 1.f, 0, MaxCharges);

		// Notify listeners (e.g., the UI or ability)
		OnChargeRecharged.Broadcast(CurrentCharges);

		// Stop the timer if charges are full
		if (CurrentCharges == MaxCharges)
		{

		}
	}
}
