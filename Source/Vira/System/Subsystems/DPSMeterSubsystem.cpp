// Fill out your copyright notice in the Description page of Project Settings.


#include "DPSMeterSubsystem.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSet.h"

/**
 * @brief Initializes the DPS meter subsystem.
 * 
 * This function sets up a timer that calls `DPSMeterTick` every 0.1 seconds.
 * 
 * @param Collection The subsystem collection.
 */
void UDPSMeterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &UDPSMeterSubsystem::DPSMeterTick, 0.1f, true);
}

/**
 * @brief Deinitializes the DPS meter subsystem.
 * 
 * This function clears the timer that calls `DPSMeterTick`.
 */
void UDPSMeterSubsystem::Deinitialize()
{
	Super::Deinitialize();
	GetWorld()->GetTimerManager().ClearTimer(TickHandle);
}

/**
 * @brief Called every tick to handle the DPS meter logic.
 * 
 * If the session is not manually handled, it checks for session timeouts.
 * The tick time is managed by "TickHandle" InRate.
 */
void UDPSMeterSubsystem::DPSMeterTick()
{
	if (!bManuallyHandleSession)
	{
		CheckSessionTimeout();
	}
}

/**
 * @brief Registers an Ability System Component (ASC) to the DPS meter.
 * 
 * This function adds the ASC to the list of registered ASCs, and subscribes to gameplay effect events.
 * 
 * @param ASC The Ability System Component to register.
 */
void UDPSMeterSubsystem::RegisterASC(UAbilitySystemComponent* ASC)
{
	if (!ASC || CombatSessions.Contains(ASC)) return;

	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDPSMeterSubsystem::OnGameplayEffectExecuted_ToSelf);
	ASC->OnGameplayEffectAppliedDelegateToTarget.AddUObject(this, &UDPSMeterSubsystem::OnGameplayEffectExecuted_ToTarget);

	CombatSessions.Add(ASC, TArray<FDPSMeterCombatSession>());
	LastDamageTimeMap.Add(ASC, 0.f);
}

/**
 * @brief Unregisters an Ability System Component (ASC) from the DPS meter.
 * 
 * This function removes the ASC from the list of registered ASCs, and unsubscribes from gameplay effect events.
 * 
 * @param ASC The Ability System Component to unregister.
 */
void UDPSMeterSubsystem::UnregisterASC(UAbilitySystemComponent* ASC)
{
	if (!ASC || !CombatSessions.Contains(ASC)) return;

	ASC->OnGameplayEffectAppliedDelegateToSelf.Clear();
	ASC->OnGameplayEffectAppliedDelegateToTarget.Clear();

	CombatSessions.FindAndRemoveChecked(ASC);
	LastDamageTimeMap.FindAndRemoveChecked(ASC);
}

/**
 * @brief Starts a new DPS meter session for the given Ability System Component (ASC).
 * 
 * @param ASC The Ability System Component to start a new session for.
 */
void UDPSMeterSubsystem::StartNewSession(UAbilitySystemComponent* ASC)
{
	TryStartNewSession(ASC);
}

/**
 * @brief Ends the current DPS meter session for the given Ability System Component (ASC).
 *
 * This function handles "Pruning" old session given the max stored sessions variable.
 * TODO: Save out the sessions so we don't have to store them in memory.
 * @param ASC The Ability System Component to end the current session for.
 */
void UDPSMeterSubsystem::EndCurrentSession(UAbilitySystemComponent* ASC)
{
	for (TTuple<UAbilitySystemComponent*, float>& Elem : LastDamageTimeMap)
	{
		float& LastDamageTime = Elem.Value;

		if (LastDamageTime > 0)
		{
			if (CombatSessions.Contains(ASC) && CombatSessions[ASC].Num() > 0)
			{
				FDPSMeterCombatSession& Session = CombatSessions[ASC].Last();
				if (Session.IsActive())
				{
					Session.EndTime = LastDamageTime;
					LastDamageTime = 0.f; // Reset for next session
					OnCurrentSessionEnded.Broadcast(Session);
				}
			}
		}
		// Prune old sessions
		if (CombatSessions[ASC].Num() > MaxStoredSessions)
		{
			CombatSessions[ASC].RemoveAt(0, CombatSessions[ASC].Num() - MaxStoredSessions);
		}
	}
}

/**
 * @brief Callback for when a gameplay effect is applied to self.
 * 
 * This function checks if the effect is a healing or damage effect, and processes it accordingly.
 * 
 * @param ASC The Ability System Component that applied the effect.
 * @param Spec The gameplay effect spec.
 * @param Handle The active gameplay effect handle.
 */
void UDPSMeterSubsystem::OnGameplayEffectExecuted_ToSelf(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
                                                         FActiveGameplayEffectHandle Handle)
{
	const FGameplayTagContainer& Tags = Spec.Def->GetAssetTags();
	UAbilitySystemComponent* InstigatorASC = Spec.GetContext().GetInstigatorAbilitySystemComponent();

	if (Tags.HasTag(HealingTag))
	{
		if (const FGameplayEffectModifiedAttribute* Attribute = Spec.GetModifiedAttribute(UGSCAttributeSet::GetHealthAttribute()))
		{
			const float Damage = FMath::Abs(Attribute->TotalMagnitude);
			//ProcessHealingTakenEvent(InstigatorASC, Spec, Damage);
		}
	}
	else if (Tags.HasTag(DamageTag))
	{
		if (const FGameplayEffectModifiedAttribute* Attribute = Spec.GetModifiedAttribute(UGSCAttributeSet::GetHealthAttribute()))
		{
			const float Damage = FMath::Abs(Attribute->TotalMagnitude);
			//ProcessDamageTakenEvent(InstigatorASC, Spec, Damage);
		}
	}

}

/**
 * @brief Callback for when a gameplay effect is applied to a target.
 * 
 * This function checks if the effect is a healing or damage effect, and processes it accordingly.
 * 
 * @param ASC The Ability System Component of the target.
 * @param Spec The gameplay effect spec.
 * @param Handle The active gameplay effect handle.
 */
void UDPSMeterSubsystem::OnGameplayEffectExecuted_ToTarget(UAbilitySystemComponent* ASC,
	const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	const FGameplayTagContainer& Tags = Spec.Def->GetAssetTags();
	UAbilitySystemComponent* InstigatorASC = Spec.GetContext().GetInstigatorAbilitySystemComponent();

	if (Tags.HasTag(DamageTag))
	{
		if (const FGameplayEffectModifiedAttribute* Attribute = Spec.GetModifiedAttribute(UGSCAttributeSet::GetHealthAttribute()))
		{
			const float Damage = FMath::Abs(Attribute->TotalMagnitude);
			ProcessDamageDoneEvent(InstigatorASC, Spec, Damage);
		}
	}
	else if (Tags.HasTag(HealingTag))
	{
		if (const FGameplayEffectModifiedAttribute* Attribute = Spec.GetModifiedAttribute(UGSCAttributeSet::GetHealthAttribute()))
		{
			const float Damage = FMath::Abs(Attribute->TotalMagnitude);
			//ProcessHealingDoneEvent(InstigatorASC, Spec, Damage);
		}
	}
}

/**
 * @brief Attempts to start a new DPS meter session for the given Ability System Component (ASC).
 * 
 * If there is an active session, it is ended first.
 * 
 * @param ASC The Ability System Component to start a new session for.
 */
void UDPSMeterSubsystem::TryStartNewSession(UAbilitySystemComponent* ASC)
{
	if (!CombatSessions.Contains(ASC)) return;

	if (CombatSessions[ASC].Num() > 0)
	{
		FDPSMeterCombatSession CurrentCombatSession = CombatSessions[ASC].Last();
		if (CurrentCombatSession.IsActive())
		{
			EndCurrentSession(ASC);
		}
	}

	
	FDPSMeterCombatSession NewSession;
	NewSession.StartTime = GetWorld()->GetTimeSeconds();
	CombatSessions[ASC].Add(NewSession);

	OnNewSessionStarted.Broadcast();
	
	UE_LOG(LogTemp, Display, TEXT("Started new DPSMeter Session"));
}

/**
 * @brief Checks for session timeouts.
 * 
 * Iterates through all registered ASCs and ends the current session if a timeout has occurred.
 * "Prunes" old session if over the MaxStoredSessions limit.
 * TODO: Save out the session to avoid storing them in memory.
 */
void UDPSMeterSubsystem::CheckSessionTimeout()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	for (auto& Elem : LastDamageTimeMap)
	{
		UAbilitySystemComponent* ASC = Elem.Key;
		float& LastDamageTime = Elem.Value;

		if (LastDamageTime > 0 && (CurrentTime - LastDamageTime) >= CombatTimeout)
		{
			if (CombatSessions.Contains(ASC) && CombatSessions[ASC].Num() > 0)
			{
				FDPSMeterCombatSession& Session = CombatSessions[ASC].Last();
				if (Session.IsActive())
				{
					Session.EndTime = LastDamageTime;
					LastDamageTime = 0.f; // Reset for next session
					OnCurrentSessionEnded.Broadcast(Session);
				}
			}
		}
		// Prune old sessions
		if (CombatSessions[ASC].Num() > MaxStoredSessions)
		{
			CombatSessions[ASC].RemoveAt(0, CombatSessions[ASC].Num() - MaxStoredSessions);
		}
	}


}

/**
 * @brief Gets the current DPS for the given Ability System Component (ASC).
 * 
 * @param ASC The Ability System Component to get the current DPS for.
 * @return The current DPS, or 0.0 if no session is active or no damage has been dealt.
 */
float UDPSMeterSubsystem::GetCurrentCombatDPS(UAbilitySystemComponent* ASC) const
{
	if (!CombatSessions.Contains(ASC) || CombatSessions[ASC].IsEmpty())
		return 0.f;
	
	const FDPSMeterCombatSession& Session = CombatSessions[ASC].Last();
	if (!Session.IsActive() || Session.TotalDamageDone <= 0.f)
		return 0.f;

	const float Duration = Session.GetDuration(GetWorld());
	return Duration > 0 ? Session.TotalDamageDone / Duration : 0.f;
}

/**
 * @brief Gets the list of combat sessions for the given Ability System Component (ASC).
 * 
 * @param ASC The Ability System Component to get the combat sessions for.
 * @return A const reference to the array of combat sessions.
 */
const TArray<FDPSMeterCombatSession>& UDPSMeterSubsystem::GetCombatSessions(UAbilitySystemComponent* ASC) const
{
	check(CombatSessions.Contains(ASC));
	return CombatSessions[ASC];
}

/**
 * @brief Gets the DPS of a specific combat session.
 * 
 * @param Session The combat session to get the DPS for.
 * @return The DPS of the session.
 */
float UDPSMeterSubsystem::GetSessionDPS(const FDPSMeterCombatSession& Session) const
{
	//If the session hasn't ended, give the current world time.
	const float EndTime = Session.EndTime <= 0.f ? GetWorld()->GetTimeSeconds() : Session.EndTime;
	return Session.TotalDamageDone / (EndTime - Session.StartTime);
}

/**
 * @brief Processes a damage done event.
 * 
 * This function updates the last damage time, starts a new session if needed, and adds the damage to the current session.
 * 
 * @param ASC The Ability System Component that dealt the damage.
 * @param Spec The gameplay effect spec.
 * @param Magnitude The amount of damage dealt.
 */
void UDPSMeterSubsystem::ProcessDamageDoneEvent(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
                                                float Magnitude)
{
	if (!CombatSessions.Contains(ASC)) return;

	TArray<FDPSMeterCombatSession>& Sessions = CombatSessions[ASC];
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// Start new session if needed
	if (Sessions.Num() == 0 || !Sessions.Last().IsActive())
	{
		if (!bManuallyHandleSession)
		{
			TryStartNewSession(ASC);
		}
	}

	// Update last damage time
	LastDamageTimeMap[ASC] = CurrentTime;

	if (Sessions.Num() <= 0 || !Sessions.Last().IsActive()) return;
	
	// Add to current session
	FDPSMeterCombatSession& CurrentSession = Sessions.Last();
	CurrentSession.TotalDamageDone += Magnitude;
	CurrentSession.DamageDoneEvents.Add(
		TPair<float, FDPSMeterEvent>(
			CurrentTime,
			FDPSMeterEvent(Magnitude,Spec.GetContext().GetAbility()->StaticClass())
			));

	OnMeterUpdated.Broadcast(CurrentSession);
}
