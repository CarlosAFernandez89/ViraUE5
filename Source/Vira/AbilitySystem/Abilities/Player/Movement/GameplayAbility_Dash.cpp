// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Dash.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGameplayAbility_Dash::UGameplayAbility_Dash(): DashCurve(nullptr), SavedGroundFriction(0),
                                                SavedBrakingDecelerationWalking(0),
                                                SavedGravityScale(1.35f), DashStartTime(0), DashDuration(0),
                                                CharacterMovementComponent(nullptr)
{
}

void UGameplayAbility_Dash::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (PlayerStateCharacter)
	{
		CharacterMovementComponent = PlayerStateCharacter->GetCharacterMovement();
	}
}

void UGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// Perform the dash
	Dash();
}

void UGameplayAbility_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{

	if (CharacterMovementComponent)
	{
		// Restore movement settings
		CharacterMovementComponent->GroundFriction = SavedGroundFriction;
		CharacterMovementComponent->BrakingDecelerationWalking = SavedBrakingDecelerationWalking;
		CharacterMovementComponent->GravityScale = SavedGravityScale;

		if(bZeroVelocityAfterDashing)
		{
			CharacterMovementComponent->Velocity = FVector::ZeroVector;
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Dash::Dash()
{
	if(CharacterMovementComponent)
	{
		// Save current movement settings
		SavedGroundFriction = CharacterMovementComponent->GroundFriction;
		SavedBrakingDecelerationWalking = CharacterMovementComponent->BrakingDecelerationWalking;
		SavedGravityScale = CharacterMovementComponent->GravityScale;
	
		// Launch the character in the dash direction
		CharacterMovementComponent->GroundFriction = 0.f;
		CharacterMovementComponent->BrakingDecelerationWalking = 0.f;
		CharacterMovementComponent->GravityScale = GravityScaleWhileDashing;

		DashStartTime = GetWorld()->GetTimeSeconds();

		// Calculate the total dash duration based on the curve's time range
		if (DashCurve)
		{
			float MinTime, MaxTime;
			DashCurve->GetTimeRange(MinTime, MaxTime);
			DashDuration = MaxTime - MinTime;
		}
		
		GetWorld()->GetTimerManager().SetTimer(ApplyDashTimer, this, &UGameplayAbility_Dash::ApplyDashMovement, GetWorld()->DeltaTimeSeconds, true);

		// Set a timer to end the dash after the specified duration
		GetWorld()->GetTimerManager().SetTimer(DashEndTimer, this, &UGameplayAbility_Dash::EndDash, DashDuration, false);

	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGameplayAbility_Dash::EndDash()
{
	// End the ability
	if(ApplyDashTimer.IsValid())
	{
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ApplyDashTimer);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Dash::ApplyDashMovement()
{
	// Calculate the current dash time
	const float CurrentTime = GetWorld()->GetTimeSeconds() - DashStartTime;
	// Check if the dash should end
	if (CurrentTime >= DashDuration)
	{
		EndDash();
		return;
	}

	// Evaluate the dash curve to get the speed multiplier
	const float SpeedMultiplier = DashCurve->GetFloatValue(CurrentTime);

	// Determine the dash direction based on where the character is facing
	FVector DashDirection = bUseMovementDirection ? GetVyraPlayerStateCharacter()->GetVelocity() : GetVyraPlayerStateCharacter()->GetActorForwardVector();

	if (UKismetMathLibrary::Vector_IsNearlyZero(DashDirection))
	{
		DashDirection = GetVyraPlayerStateCharacter()->GetActorForwardVector();
	}
	
	DashDirection.Normalize();

	// Calculate the dash velocity based on the curve evaluation
	FVector DashVelocity = DashDirection * DashDistance * SpeedMultiplier / DashDuration;
	DashVelocity.Z = 0.0f;

	// Apply the velocity to the character
	CharacterMovementComponent->Velocity = DashVelocity;
}
