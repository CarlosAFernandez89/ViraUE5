// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_WithCastTime.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

void UVyraGameplayAbility_WithCastTime::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UVyraGameplayAbility_WithCastTime::StartCasting()
{
	WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, CastTime);
	WaitDelay->OnFinish.AddDynamic(this, &UVyraGameplayAbility_WithCastTime::OnFinishCasting);
	WaitDelay->ReadyForActivation();
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "CastingSucceeded");
	}
}
