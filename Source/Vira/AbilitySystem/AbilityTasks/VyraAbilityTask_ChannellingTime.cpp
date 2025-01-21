// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAbilityTask_ChannellingTime.h"

UVyraAbilityTask_ChannellingTime* UVyraAbilityTask_ChannellingTime::StartChannelingTime(UGameplayAbility* OwningAbility)
{
	UVyraAbilityTask_ChannellingTime* MyTask = NewAbilityTask<UVyraAbilityTask_ChannellingTime>(OwningAbility);
	return MyTask;
}

void UVyraAbilityTask_ChannellingTime::StopChanneling()
{
	if (bIsChanneling && Ability)
	{
		// Calculate the total channeling time
		float TotalChannelTime = GetWorld()->GetTimeSeconds() - StartTime;

		// Broadcast the channeling time
		OnChannelingCompleted.Broadcast(TotalChannelTime);

		// End the task
		EndTask();
	}
}

void UVyraAbilityTask_ChannellingTime::Activate()
{
	Super::Activate();

	// Start counting time
	if (Ability)
	{
		StartTime = GetWorld()->GetTimeSeconds(); // Record the start time
		bIsChanneling = true;
	}
	else
	{
		EndTask(); // End the task if the ability is invalid
	}
}

void UVyraAbilityTask_ChannellingTime::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);

	// Ensure we stop counting when the task is destroyed
	bIsChanneling = false;
}
