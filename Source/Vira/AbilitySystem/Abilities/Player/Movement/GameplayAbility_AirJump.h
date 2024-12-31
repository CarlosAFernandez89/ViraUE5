// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_AirJump.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_AirJump : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_AirJump();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Player|Movement")
	bool ActivateAirJump();

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility|Player|Movement")
	bool CanAirJump() const;

private:

	UFUNCTION()
	void OnLandedEvent(const FHitResult& LandHit);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump")
	float AirJumpVelocity = 500.0f;

private:

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	bool bFoundAttribute;
	float StartingAirJumpCount;
	float AirJumpsRemaining;
};
