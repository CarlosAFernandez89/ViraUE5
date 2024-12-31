// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_AirJump.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"

UGameplayAbility_AirJump::UGameplayAbility_AirJump(): MovementComponent(nullptr), bFoundAttribute(false),
                                                      StartingAirJumpCount(0),
                                                      AirJumpsRemaining(0)
{
}

void UGameplayAbility_AirJump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	GetVyraPlayerStateCharacter()->LandedDelegate.AddDynamic(this, &UGameplayAbility_AirJump::OnLandedEvent);
	MovementComponent = GetVyraPlayerStateCharacter()->GetCharacterMovement();
}

void UGameplayAbility_AirJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	bFoundAttribute = false;
	StartingAirJumpCount = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UCombatAttributeSet::GetAirJumpCountAttribute(),bFoundAttribute);
	if(!bFoundAttribute)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get AirJumpCount Attribute."));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	AirJumpsRemaining = StartingAirJumpCount;
}

void UGameplayAbility_AirJump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGameplayAbility_AirJump::ActivateAirJump()
{
	if(CanAirJump())
	{
		// Perform the air jump
		GetVyraPlayerStateCharacter()->LaunchCharacter(FVector(0.0f, 0.0f, AirJumpVelocity), false, true);
		AirJumpsRemaining -= 1.f;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UGameplayAbility_AirJump::ActivateAirJump - Maximum number of air jumps reached."));
	}
	return false;
}

bool UGameplayAbility_AirJump::CanAirJump() const
{
	return !MovementComponent->IsMovingOnGround() && AirJumpsRemaining > 0;
}

void UGameplayAbility_AirJump::OnLandedEvent(const FHitResult& LandHit)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
