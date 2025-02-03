// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DPSMeterSubsystem.generated.h"


// Detailed event tracking
USTRUCT(BlueprintType)
struct FDPSMeterEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float Amount = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> SourceAbility = nullptr;

};

// Container for each combat "session"
USTRUCT(BlueprintType)
struct FDPSMeterCombatSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float StartTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float EndTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalDamageDone = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float TotalDamageTaken = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalHealingDone = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float TotalHealingTaken = 0.f;
	
	TArray<TPair<float, FDPSMeterEvent>> DamageDoneEvents; // Timestamp, Amount
	TArray<TPair<float, FDPSMeterEvent>> DamageTakenEvents; // Timestamp, Amount

	TArray<TPair<float, FDPSMeterEvent>> HealingDoneEvents; // Timestamp, Amount
	TArray<TPair<float, FDPSMeterEvent>> HealingTakenEvents; // Timestamp, Amount


	bool IsActive() const { return EndTime == 0.f; }
	float GetDuration(const UWorld* World) const 
	{ 
		return IsActive() ? (World->GetTimeSeconds() - StartTime) : (EndTime - StartTime);
	}

	bool operator==(const FDPSMeterCombatSession& Other) const
	{
		return StartTime == Other.StartTime && EndTime == Other.EndTime;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDPSMeterCombatSession& Struct)
{
	uint32 Hash = FCrc::MemCrc32(&Struct, sizeof(FDPSMeterCombatSession));
	return Hash;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeterUpdated, FDPSMeterCombatSession, CurrentSession);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewSessionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentSessionEnded, FDPSMeterCombatSession, CurrentSession);


/**
 * 
 */
UCLASS()
class VIRA_API UDPSMeterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	void DPSMeterTick();

	UFUNCTION(BlueprintCallable)
	float GetCurrentCombatDPS(UAbilitySystemComponent* ASC) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FDPSMeterCombatSession>& GetCombatSessions(UAbilitySystemComponent* ASC) const;

	UFUNCTION(BlueprintCallable)
	float GetSessionDPS(const FDPSMeterCombatSession& Session) const;
	
	// Register an ASC to track its damage/healing
	UFUNCTION(BlueprintCallable)
	void RegisterASC(class UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintCallable)
	void UnregisterASC(class UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxStoredSessions() const { return MaxStoredSessions; };
	
	UFUNCTION(BlueprintCallable)
	void SetMaxStoredSessions(const int32 NewMaxStoredSessions){ MaxStoredSessions = NewMaxStoredSessions; };

	UFUNCTION(BlueprintCallable, Category="DPSMeterSubsystem")
	void SetManuallyHandleSession(const bool bNewState)
	{
		bManuallyHandleSession = bNewState;
	}
	
	UFUNCTION(BlueprintCallable, Category="DPSMeterSubsystem")
	void StartNewSession(class UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintCallable, Category="DPSMeterSubsystem")
	void EndCurrentSession(class UAbilitySystemComponent* ASC);

public:
	UPROPERTY(BlueprintAssignable, Category="DPSMeterSubsystem")
	FOnMeterUpdated OnMeterUpdated;

	UPROPERTY(BlueprintAssignable, Category="DPSMeterSubsystem")
	FOnNewSessionStarted OnNewSessionStarted;

	UPROPERTY(BlueprintAssignable, Category="DPSMeterSubsystem")
	FOnCurrentSessionEnded OnCurrentSessionEnded;
private:
	void OnGameplayEffectExecuted_ToSelf(class UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void OnGameplayEffectExecuted_ToTarget(class UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	void TryStartNewSession(UAbilitySystemComponent* ASC);
	void CheckSessionTimeout();

	void ProcessDamageDoneEvent(UAbilitySystemComponent* InstigatorASC, const FGameplayEffectSpec& Spec, float Magnitude);
	//void ProcessDamageTakenEvent(UAbilitySystemComponent* InstigatorASC, const FGameplayEffectSpec& Spec, float Magnitude);

	//void ProcessHealingTakenEvent(UAbilitySystemComponent* InstigatorASC, const FGameplayEffectSpec& Spec, float Magnitude);
	//void ProcessHealingDoneEvent(UAbilitySystemComponent* InstigatorASC, const FGameplayEffectSpec& Spec, float Magnitude);
	
	TMap<UAbilitySystemComponent*, TArray<FDPSMeterCombatSession>> CombatSessions;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	bool bManuallyHandleSession = false;
	
	FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag("GameplayEffect.Damage");
	FGameplayTag HealingTag = FGameplayTag::RequestGameplayTag("GameplayEffect.Healing");

	UPROPERTY()
	TMap<UAbilitySystemComponent*, float> LastDamageTimeMap;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float CombatTimeout = 10.f; // Seconds without damage to end combat

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 MaxStoredSessions = 20;
	
	UPROPERTY()
	FTimerHandle TickHandle;
};
