// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_Channelling.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/AbilitySystem/AbilityTasks/VyraAbilityTask_ChannellingTime.h"

UVyraGameplayAbility_Channelling::UVyraGameplayAbility_Channelling(): AbilityTask(nullptr), InputReleased(nullptr)
{
	MessageTag = FGameplayTag::RequestGameplayTag("UIMessaging.Ability.Channelling");
	ChannelStartTimeSectionName = FName("ChannelStartTime");
	ChannelEndSectionName = FName("LoopEnd");
}

void UVyraGameplayAbility_Channelling::OnInputRelease(float TimeHeld)
{
	if (InputReleased && InputReleased->IsActive())
	{
		InputReleased->EndTask();
	}
	
	if (AbilityTask && AbilityTask->IsActive())
	{
		AbilityTask->StopChanneling();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UVyraGameplayAbility_Channelling::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bUseRandomPerfectRange)
	{
		PerfectChannelRange.Y = UKismetMathLibrary::RandomFloatInRange(MaxChannelDuration * 0.3f, MaxChannelDuration);
		PerfectChannelRange.X = UKismetMathLibrary::RandomFloatInRange(PerfectChannelRange.Y / 2.f, PerfectChannelRange.Y - MaxChannelDuration * 0.1f);
	}

	InputReleased = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (InputReleased)
	{
		InputReleased->OnRelease.AddDynamic(this, &UVyraGameplayAbility_Channelling::OnInputRelease);
		InputReleased->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	const float ChannellingStartTime = GetSectionStartTime(ChannelStartTimeSectionName, bIsAttackSpeedAbility ? GetAttackSpeed() : GetCastSpeed());
	GetWorld()->GetTimerManager().SetTimer(ChannellingTimerHandle, this, &UVyraGameplayAbility_Channelling::StartChannelling, ChannellingStartTime);
}

void UVyraGameplayAbility_Channelling::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ChannellingTimerHandle);

	if (AbilityTask && AbilityTask->IsActive())
	{
		AbilityTask->EndTask();
	}

	if (InputReleased && InputReleased->IsActive())
	{
		InputReleased->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVyraGameplayAbility_Channelling::OnNormalChannelling_Implementation(float TotalChannelTime)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Normal Channeling Activated"));
	}
}

void UVyraGameplayAbility_Channelling::OnPerfectChannelling_Implementation(float TotalChannelTime)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Perfect Channeling Activated"));
	}
}

void UVyraGameplayAbility_Channelling::StartChannelling_Implementation()
{
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UGameplayMessageSubsystem* MessageSubsystem = GameInstance->GetSubsystem<UGameplayMessageSubsystem>())
		{
			FVyraChannelingMessage Message;
			Message.AbilityName = AbilityInfo.Name;
			Message.MaxChannelDuration = MaxChannelDuration;
			Message.PerfectChannelRange = PerfectChannelRange;
			MessageSubsystem->BroadcastMessage(MessageTag, Message);
		}
	}

	AbilityTask = UVyraAbilityTask_ChannellingTime::StartChannelingTime(this);
	AbilityTask->OnChannelingCompleted.AddDynamic(this, &UVyraGameplayAbility_Channelling::OnChannellingComplete);
	AbilityTask->ReadyForActivation();
	
}

void UVyraGameplayAbility_Channelling::OnChannellingComplete_Implementation(float TotalChannelTime)
{
	const bool bWasPerfectChannel = WasChannelInPerfectRange(TotalChannelTime);
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UGameplayMessageSubsystem* MessageSubsystem = GameInstance->GetSubsystem<UGameplayMessageSubsystem>())
		{
			FVyraChannelingMessage_Released Message;
			Message.bWasPerfectChannel = bWasPerfectChannel;
			MessageSubsystem->BroadcastMessage(MessageTag, Message);
		}
	}
	
	if (bWasPerfectChannel)
	{
		OnPerfectChannelling(TotalChannelTime);
	}
	else
	{
		OnNormalChannelling(TotalChannelTime);
	}

	if (MontageToPlay)
	{
		GetVyraAnimInstance()->Montage_JumpToSection(ChannelEndSectionName, MontageToPlay);
	}
}

float UVyraGameplayAbility_Channelling::GetSectionStartTime(const FName SectionName, const float PlayRate) const
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

	return 0.f;
}

bool UVyraGameplayAbility_Channelling::WasChannelInPerfectRange(float TotalChannelTime) const
{
	return TotalChannelTime >= PerfectChannelRange.X && TotalChannelTime <= PerfectChannelRange.Y;
}
