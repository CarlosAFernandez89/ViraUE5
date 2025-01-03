// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityCharm_CheatDeath.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/GSCCoreComponent.h"

UGameplayAbilityCharm_CheatDeath::UGameplayAbilityCharm_CheatDeath()
	:Super()
{
}

void UGameplayAbilityCharm_CheatDeath::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if(UGSCCoreComponent* CoreComponent = GetVyraGSCCoreComponent())
	{
		CoreComponent->OnDeath.AddDynamic(this, &UGameplayAbilityCharm_CheatDeath::OnDeathEvent);
	}

}

void UGameplayAbilityCharm_CheatDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbilityCharm_CheatDeath::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbilityCharm_CheatDeath::OnCommitAbilityFailed_Implementation()
{
	
}


void UGameplayAbilityCharm_CheatDeath::OnDeathEvent_Implementation()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActorFromActorInfo(),
		FGameplayTag::RequestGameplayTag("CharmAbility.CheatDeath"),
		FGameplayEventData()
		);
}

