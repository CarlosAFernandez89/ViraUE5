// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_Jump : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Jump();
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void OnPlayerLanded(const FHitResult& LandedHitResult);
	
private:


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Ability|Movement", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float JumpZVelocity = 420.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Ability|Movement", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float AirControl = 0.85f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Ability|Movement", meta = (ClampMin = "0.0", ClampMax = "0.5", UIMin = "0.0", UIMax = "0.5"))
	float MaxJumpHoldTime = 0.35f;
};
