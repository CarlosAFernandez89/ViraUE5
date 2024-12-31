// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_CoyoteJump.h"

#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UGameplayAbility_CoyoteJump::UGameplayAbility_CoyoteJump(): MovementComponent(nullptr)
{
}

void UGameplayAbility_CoyoteJump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	MovementComponent = GetVyraPlayerStateCharacter()->GetCharacterMovement();
	
}

void UGameplayAbility_CoyoteJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UGameplayAbility_CoyoteJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_CoyoteJump::ActivateCoyoteJump()
{
	if(!MovementComponent->IsMovingOnGround())
	{
		GetVyraPlayerStateCharacter()->LaunchCharacter(FVector(0.0f, 0.0f, CoyoteJumpVelocity), false, true);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}