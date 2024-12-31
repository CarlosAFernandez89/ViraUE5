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
		// Filter out the vertical input (Y component) to ensure only horizontal movement
		const float HorizontalInput = MovementInput.X;

		// Only move if the horizontal input exceeds a small dead zone threshold
		constexpr float DeadZone = 0.2f; // Adjust this value based on the controller sensitivity

		if (FMath::Abs(HorizontalInput) > DeadZone)
		{
			// Move along the world right vector (horizontal movement in world coordinates)
			PlayerStateCharacter->AddMovementInput(FVector::RightVector, HorizontalInput);
		}
	}
}
