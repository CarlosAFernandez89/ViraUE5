// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility_WithCharges.h"
#include "GameplayAbility_Dash.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_Dash : public UVyraGameplayAbility_WithCharges
{
	GENERATED_BODY()

public:
	UGameplayAbility_Dash();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	// Function that applies the dash movement
	void Dash();

	// Function that ends the dash
	void EndDash();

	void ApplyDashMovement();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	bool bUseMovementDirection = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	class UCurveFloat* DashCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashDistance = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float GravityScaleWhileDashing = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	bool bZeroVelocityAfterDashing = false;
	
private:
	float SavedGroundFriction;
	float SavedBrakingDecelerationWalking;
	float SavedGravityScale;
	float DashStartTime;
	float DashDuration;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY()
	FTimerHandle ApplyDashTimer;

	// A timer handle to manage the duration of the dash
	FTimerHandle DashEndTimer;

};
