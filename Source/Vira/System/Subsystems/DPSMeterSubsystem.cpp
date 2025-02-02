// Fill out your copyright notice in the Description page of Project Settings.


#include "DPSMeterSubsystem.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSet.h"

void UDPSMeterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &UDPSMeterSubsystem::DPSMeterTick, 0.1f, true);
}

void UDPSMeterSubsystem::Deinitialize()
{
	Super::Deinitialize();
	GetWorld()->GetTimerManager().ClearTimer(TickHandle);
}

void UDPSMeterSubsystem::DPSMeterTick()
{
	if (!bManuallyHandleSession)
	{
		CheckSessionTimeout();
	}
}

void UDPSMeterSubsystem::RegisterASC(class UAbilitySystemComponent* ASC)
{
	if (!ASC || CombatSessions.Contains(ASC)) return;

	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDPSMeterSubsystem::OnGameplayEffectExecuted_ToSelf);
	ASC->OnGameplayEffectAppliedDelegateToTarget.AddUObject(this, &UDPSMeterSubsystem::OnGameplayEffectExecuted_ToTarget);

	CombatSessions.Add(ASC, TArray<FDPSMeterCombatSession>());
	LastDamageTimeMap.Add(ASC, 0.f);
}

void UDPSMeterSubsystem::UnregisterASC(class UAbilitySystemComponent* ASC)
{
	if (!ASC || !CombatSessions.Contains(ASC)) return;

	ASC->OnGameplayEffectAppliedDelegateToSelf.Clear();
	ASC->OnGameplayEffectAppliedDelegateToTarget.Clear();

	CombatSessions.FindAndRemoveChecked(ASC);
	LastDamageTimeMap.FindAndRemoveChecked(ASC);
}


void UDPSMeterSubsystem::StartNewSession(class UAbilitySystemComponent* ASC)
{
	TryStartNewSession(ASC);
}

void UDPSMeterSubsystem::EndCurrentSession(class UAbilitySystemComponent* ASC)
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

void UDPSMeterSubsystem::OnGameplayEffectExecuted_ToSelf(class UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
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


void UDPSMeterSubsystem::OnGameplayEffectExecuted_ToTarget(class UAbilitySystemComponent* ASC,
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

const TArray<FDPSMeterCombatSession>& UDPSMeterSubsystem::GetCombatSessions(UAbilitySystemComponent* ASC) const
{
	check(CombatSessions.Contains(ASC));
	return CombatSessions[ASC];
}

float UDPSMeterSubsystem::GetSessionDPS(const FDPSMeterCombatSession& Session) const
{
	//If the session hasn't ended, give the current world time.
	const float EndTime = Session.EndTime <= 0.f ? GetWorld()->GetTimeSeconds() : Session.EndTime;
	return Session.TotalDamageDone / (EndTime - Session.StartTime);
}

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
