// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_CoyoteJump.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_CoyoteJump : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_CoyoteJump();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Player|Movement")
	void ActivateCoyoteJump();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump")
	float CoyoteJumpVelocity = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump")
	float CoyoteJumpDuration = 0.25f;

private:

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
};
