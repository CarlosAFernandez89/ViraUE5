// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_RangedAttack.h"

UGameplayAbility_RangedAttack::UGameplayAbility_RangedAttack()
{
}

void UGameplayAbility_RangedAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbility_RangedAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

void UGameplayAbility_RangedAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

EAttackDirection UGameplayAbility_RangedAttack::GetAttackDirection(const FVector2D& MovementInput)
{
	// Normalize the input vector to get a unit vector representing the direction.
	FVector2D NormalizedInput = MovementInput.GetSafeNormal();

	// Use a threshold to determine the primary direction.
	// A threshold of 0.5 means that the angle must be within 45 degrees of the axis.
	constexpr float Threshold = 0.5f;

	if (FMath::Abs(NormalizedInput.X) > Threshold)
	{
		return EAttackDirection::Horizontal;
	}
	else if (NormalizedInput.Y > Threshold)
	{
		return EAttackDirection::Up;
	}
	else if (NormalizedInput.Y < -Threshold)
	{
		return EAttackDirection::Down;
	}
	else
	{
		// Default case (e.g., if the input vector is very small or zero)
		return EAttackDirection::Horizontal;
	}
}
