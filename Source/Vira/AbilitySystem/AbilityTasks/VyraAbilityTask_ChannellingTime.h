// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "VyraAbilityTask_ChannellingTime.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChannelingCompleted, float, TotalChannelTime);

/**
 * 
 */
UCLASS()
class VIRA_API UVyraAbilityTask_ChannellingTime : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/** Broadcast when channeling is stopped */
	UPROPERTY(BlueprintAssignable)
	FOnChannelingCompleted OnChannelingCompleted;

	/**
	 * Starts the channeling timer.
	 * @param OwningAbility The owning gameplay ability.
	 * @return The created ability task.
	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Channeling Timer", HideFromNodeList), Category = "Ability|Tasks")
	static UVyraAbilityTask_ChannellingTime* StartChannelingTime(UGameplayAbility* OwningAbility);

	/**
	 * Stops the channeling timer and triggers the OnChannelingCompleted event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks")
	void StopChanneling();

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	/** The time when the channeling started */
	float StartTime;

	/** Whether the task is currently running */
	bool bIsChanneling;
};
