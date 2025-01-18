// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_WithCastTime.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

UVyraGameplayAbility_WithCastTime::UVyraGameplayAbility_WithCastTime(): WaitDelay(nullptr)
{
	MessageTag = FGameplayTag::RequestGameplayTag("UIMessaging.Ability.Cast");
}

void UVyraGameplayAbility_WithCastTime::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	// We call CommitAbility After Casting is Successful so we can apply the cost and cooldown there.
	StartCasting();
}

void UVyraGameplayAbility_WithCastTime::StartCasting()
{
	CastTime = GetSectionStartTime(EndCastTimeSectionName, GetCastSpeed());

	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UGameplayMessageSubsystem* MessageSubsystem = GameInstance->GetSubsystem<UGameplayMessageSubsystem>())
		{
			FVyraCastTimeMessage Message;
			Message.AbilityName = AbilityInfo.Name;
			Message.CastDuration = CastTime;
			MessageSubsystem->BroadcastMessage(MessageTag, Message);
		}
	}
	
	WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, CastTime);
	WaitDelay->OnFinish.AddDynamic(this, &UVyraGameplayAbility_WithCastTime::OnFinishCasting);
	WaitDelay->ReadyForActivation();

	OnCastingStarted();
}

void UVyraGameplayAbility_WithCastTime::OnFinishCasting()
{
	OnCastingSucceeded();
}

float UVyraGameplayAbility_WithCastTime::GetSectionStartTime(const FName SectionName, const float PlayRate) const
{
	if (MontageToPlay)
	{
		for (const FCompositeSection& Section : MontageToPlay->CompositeSections)
		{
			if (Section.SectionName == SectionName)
			{
				return (Section.GetTime(EAnimLinkMethod::Type::Absolute) / PlayRate);
			}
		}
	}

	return -1.f;
}

void UVyraGameplayAbility_WithCastTime::OnCastingSucceeded_Implementation()
{	
	if (!CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		// If the ability cannot be committed, end it here
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
}
