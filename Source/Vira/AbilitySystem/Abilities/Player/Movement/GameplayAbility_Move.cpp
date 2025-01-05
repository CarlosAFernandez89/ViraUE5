// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Move.h"

#include "Kismet/KismetMathLibrary.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"

UGameplayAbility_Move::UGameplayAbility_Move()
{
}

void UGameplayAbility_Move::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGameplayAbility_Move::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_Move::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Move::Move(const FVector2D& MovementInput)
{
	if (!MovementInput.IsNearlyZero() && PlayerStateCharacter)
	{
		// Normalize the movement input
		FVector2D NormalizedInput = MovementInput.GetSafeNormal();

		// World space movement
		PlayerStateCharacter->AddMovementInput(FVector::ForwardVector, NormalizedInput.Y); // Y is forward/backward in world space
		PlayerStateCharacter->AddMovementInput(FVector::RightVector, NormalizedInput.X); // X is right/left in world space
	}
}
