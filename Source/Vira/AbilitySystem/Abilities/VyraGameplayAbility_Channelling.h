// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraGameplayAbility.h"
#include "VyraGameplayAbility_Channelling.generated.h"

class UAbilityTask_WaitInputRelease;
class UVyraAbilityTask_ChannellingTime;

USTRUCT(BlueprintType)
struct FVyraChannelingMessage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AbilityName = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxChannelDuration = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PerfectChannelRange = FVector2D();
};

USTRUCT(BlueprintType)
struct FVyraChannelingMessage_Released
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWasPerfectChannel = false;
};

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_Channelling : public UVyraGameplayAbility
{
	GENERATED_BODY()

protected:
	UVyraGameplayAbility_Channelling();


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnChannellingComplete(float TotalChannelTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnNormalChannelling(float TotalChannelTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnPerfectChannelling(float TotalChannelTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void StartChannelling();
	
private:
	
	float GetSectionStartTime(const FName SectionName, const float PlayRate) const;

	UFUNCTION()
	bool WasChannelInPerfectRange(float TotalChannelTime) const;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Channell")
	FGameplayTag MessageTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.5f, ClampMax = 10.f), Category ="Channell")
	float MaxChannelDuration = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.5f, ClampMax = 10.f), Category ="Channell")
	FVector2D PerfectChannelRange = FVector2D(2.2f, 2.6f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Channell")
	bool bUseRandomPerfectRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Channell")
	FName ChannelStartTimeSectionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Channell")
    FName ChannelEndSectionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Channell")
	bool bIsAttackSpeedAbility = false;

private:

	UPROPERTY()
	FTimerHandle ChannellingTimerHandle;

	UPROPERTY()
	UVyraAbilityTask_ChannellingTime* AbilityTask;

	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputReleased;
};
