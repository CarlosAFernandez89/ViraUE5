// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Jump.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"

UGameplayAbility_Jump::UGameplayAbility_Jump()
{
}

void UGameplayAbility_Jump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	PlayerStateCharacter->JumpMaxHoldTime = MaxJumpHoldTime;
	PlayerStateCharacter->LandedDelegate.AddDynamic(this, &UGameplayAbility_Jump::OnPlayerLanded);
	
	if (UCharacterMovementComponent* MovementComponent = PlayerStateCharacter->GetCharacterMovement())
	{
		MovementComponent->JumpZVelocity = JumpZVelocity;
		MovementComponent->AirControl = AirControl;
	}
}

void UGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// If the ability cannot be committed, end it here
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
}

void UGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Jump::OnPlayerLanded(const FHitResult& LandedHitResult)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
