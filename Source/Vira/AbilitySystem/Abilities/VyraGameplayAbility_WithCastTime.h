// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraGameplayAbility.h"
#include "VyraGameplayAbility_WithCastTime.generated.h"

USTRUCT(BlueprintType)
struct FVyraCastTimeMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastDuration = 0.0f;
};

class UAbilityTask_WaitDelay;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_WithCastTime : public UVyraGameplayAbility
{
	GENERATED_BODY()

protected:
	UVyraGameplayAbility_WithCastTime();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void StartCasting();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCastingStarted();
	
	UFUNCTION()
	void OnFinishCasting();

	UFUNCTION(BlueprintNativeEvent)
	void OnCastingSucceeded();

	UFUNCTION(BlueprintCallable)
	float GetSectionStartTime(const FName SectionName, const float PlayRate = 1.f) const;

protected:
	UPROPERTY(BlueprintReadWrite, Category ="CastTime")
	UAbilityTask_WaitDelay* WaitDelay;
	
	UPROPERTY(BlueprintReadOnly, Category ="CastTime")
	float CastTime = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="CastTime")
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="CastTime")
	FName EndCastTimeSectionName = "CastTimeEnd";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="CastTime")
	FGameplayTag MessageTag;
};
