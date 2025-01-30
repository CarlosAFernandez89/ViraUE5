// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetCriticalStrikeChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,1.f);
	}
	else if(Attribute == GetCriticalStrikeDamageMultiplierAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, 200.f);
	}
	else if(Attribute == GetCastSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 2.f);
	}
	else if(Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 2.f);
	}
	else if(Attribute == GetDamageReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 0.75f);
	}
	else if(Attribute == GetBaseDamageAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetAirJumpCountAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 850.f);
	}
	else if(Attribute == GetKnockBackResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if(Attribute == GetGlobalCooldownReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, 1.f);
	}
}

void UCombatAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if(Attribute == GetMovementSpeedAttribute())
		{
			if(UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				MovementComponent->MaxWalkSpeed = NewValue;
				OnCombatSetMovementSpeedChanged.Broadcast();
			}
		}
	}
}

void UCombatAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGSCAttributeSetExecutionData ExecutionData;
	GetExecutionDataFromMod(Data, ExecutionData);
	
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, BaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, CastSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, CriticalStrikeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, CriticalStrikeDamageMultiplier, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, KnockBackResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, AirJumpCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, GlobalCooldownReduction, COND_None, REPNOTIFY_Always);


}

void UCombatAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetCriticalStrikeChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,1.f);
	}
	else if(Attribute == GetCriticalStrikeDamageMultiplierAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, 200.f);
	}
	else if(Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 2.f);
	}
	else if(Attribute == GetCastSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 2.f);
	}
	else if(Attribute == GetDamageReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 0.9f);
	}
	else if(Attribute == GetBaseDamageAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetAirJumpCountAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1500.f);
	}
	else if(Attribute == GetKnockBackResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if(Attribute == GetGlobalCooldownReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, 1.f);
	}
}

void UCombatAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, BaseDamage, OldBaseDamage);
}

void UCombatAttributeSet::OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, CastSpeed, OldCastSpeed);
}

void UCombatAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UCombatAttributeSet::OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, CriticalStrikeChance, OldCriticalStrikeChance);
}

void UCombatAttributeSet::OnRep_CriticalStrikeDamageMultiplier(
	const FGameplayAttributeData& OldCriticalStrikeDamageMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, CriticalStrikeDamageMultiplier, OldCriticalStrikeDamageMultiplier);
}

void UCombatAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, DamageReduction, OldDamageReduction);

}

void UCombatAttributeSet::OnRep_KnockBackResistance(const FGameplayAttributeData& OldKnockBackResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, KnockBackResistance, OldKnockBackResistance);
}

void UCombatAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UCombatAttributeSet::OnRep_AirJumpCount(const FGameplayAttributeData& OldAirJumpCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, AirJumpCount, OldAirJumpCount);
}
void UCombatAttributeSet::OnRep_GlobalCooldownReduction(const FGameplayAttributeData& OldGlobalCooldownReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, GlobalCooldownReduction, OldGlobalCooldownReduction);

}