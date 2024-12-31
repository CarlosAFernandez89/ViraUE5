// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility.h"

#include "Vira/Character/VyraPlayerStateCharacter.h"

UVyraGameplayAbility::UVyraGameplayAbility(): PlayerStateCharacter(nullptr)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVyraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	PlayerStateCharacter = Cast<AVyraPlayerStateCharacter>(GetAvatarActorFromActorInfo());
	
}
