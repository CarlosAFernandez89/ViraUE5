﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityTask_WaitForInteractableTargets.h"
#include "Vira/Interaction/InteractionQuery.h"

#include "AbilityTask_WaitForInteractableTargets_SingleLineTrace.generated.h"


struct FCollisionProfileName;

class UGameplayAbility;
class UObject;
struct FFrame;

/**
 * 
 */
UCLASS()
class VIRA_API UAbilityTask_WaitForInteractableTargets_SingleLineTrace : public UAbilityTask_WaitForInteractableTargets
{
	GENERATED_UCLASS_BODY()

	virtual void Activate() override;

	/** Wait until we trace new set of interactables.  This task automatically loops. */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitForInteractableTargets_SingleLineTrace* WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FInteractionQuery InteractionQuery, FCollisionProfileName TraceProfile, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange = 100, float InteractionScanRate = 0.100, bool bShowDebug = false);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformTrace();

	UPROPERTY()
	FInteractionQuery InteractionQuery;

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
