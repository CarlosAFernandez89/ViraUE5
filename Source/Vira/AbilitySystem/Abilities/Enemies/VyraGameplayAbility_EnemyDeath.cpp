// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_EnemyDeath.h"

#include "Components/GSCCoreComponent.h"
#include "Vira/NPC/VyraNPCWithASC.h"

UVyraGameplayAbility_EnemyDeath::UVyraGameplayAbility_EnemyDeath(): GSCCoreComponent(nullptr)
{
}

void UVyraGameplayAbility_EnemyDeath::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                    const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
	{
		if (AVyraNPCWithASC* OwningActor = Cast<AVyraNPCWithASC>(AvatarActor))
		{
			GSCCoreComponent = OwningActor->GetGSCCoreComponent();
			GSCCoreComponent->OnDeath.AddDynamic(this, &UVyraGameplayAbility_EnemyDeath::OnDeathEvent);
		}
	}
}

void UVyraGameplayAbility_EnemyDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UVyraGameplayAbility_EnemyDeath::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVyraGameplayAbility_EnemyDeath::OnDeathEvent()
{
	GSCCoreComponent->OnDeath.RemoveDynamic(this, &UVyraGameplayAbility_EnemyDeath::OnDeathEvent);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(GetAssetTags().First());
	UGSCGameplayAbility* ActivatedAbility;
	GSCCoreComponent->ActivateAbilityByTags(TagContainer, ActivatedAbility);
}
