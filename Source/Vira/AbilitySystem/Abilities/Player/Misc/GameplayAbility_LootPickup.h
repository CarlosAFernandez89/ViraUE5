// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_LootPickup.generated.h"

class ACurrencyDropBase;
/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_LootPickup : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_LootPickup();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|Misc")
	void PullActorsToPlayer(TSubclassOf<ACurrencyDropBase> CurrencyClass, float InRadius);
	
private:
	void PullCurrencyLoop();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem|Misc")
	float LerpSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem|Misc")
	TArray<TSubclassOf<ACurrencyDropBase>> CurrencyToPickUp;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem|Misc")
	bool bDebug = false;
private:
	FTimerHandle PullTimerHandle;
};
