// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_MeleeAttack.generated.h"


UENUM(BlueprintType)
enum EAttackDirection : uint8
{
	Horizontal,
	Up,
	Down
};
/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_MeleeAttack : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_MeleeAttack();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintPure)
	static EAttackDirection GetAttackDirection(const FVector2D& MovementInput);

};
