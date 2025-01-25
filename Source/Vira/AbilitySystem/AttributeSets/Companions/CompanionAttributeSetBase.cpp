// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionAttributeSetBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Net/UnrealNetwork.h"

UCompanionAttributeSetBase::UCompanionAttributeSetBase()
{
}

void UCompanionAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetCriticalStrikeChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,1.f);
	}
	else if(Attribute == GetCriticalStrikeDamageMultiplierAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetCastSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 3.f);
	}
	else if(Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.05f, 3.f);
	}
	else if(Attribute == GetDamageReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 0.9f);
	}
	else if(Attribute == GetBaseDamageAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
	}
	else if(Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 2500.f);
	}
	else if(Attribute == GetKnockBackResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

void UCompanionAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(const FGameplayAbilityActorInfo* ActorInfo = GetActorInfo())
	{
		if(Attribute == GetMovementSpeedAttribute())
		{
			if(UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(ActorInfo->MovementComponent))
			{
				MovementComponent->MaxWalkSpeed = NewValue;
				OnCompanionMovementSpeedChanged.Broadcast();
			}
		}
	}
}

void UCompanionAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGSCAttributeSetExecutionData ExecutionData;
	GetExecutionDataFromMod(Data, ExecutionData);
}

void UCompanionAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, BaseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, CastSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, CriticalStrikeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, CriticalStrikeDamageMultiplier, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, KnockBackResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCompanionAttributeSetBase, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UCompanionAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetCriticalStrikeChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,1.f);
	}
	else if(Attribute == GetCriticalStrikeDamageMultiplierAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.f, UE_BIG_NUMBER);
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
	else if(Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1500.f);
	}
	else if(Attribute == GetKnockBackResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
}

void UCompanionAttributeSetBase::OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, BaseDamage, OldBaseDamage);
}

void UCompanionAttributeSetBase::OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, CastSpeed, OldCastSpeed);
}

void UCompanionAttributeSetBase::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, AttackSpeed, OldAttackSpeed);
}

void UCompanionAttributeSetBase::OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, CriticalStrikeChance, OldCriticalStrikeChance);
}

void UCompanionAttributeSetBase::OnRep_CriticalStrikeDamageMultiplier(
	const FGameplayAttributeData& OldCriticalStrikeDamageMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, CriticalStrikeDamageMultiplier, OldCriticalStrikeDamageMultiplier);
}

void UCompanionAttributeSetBase::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, DamageReduction, OldDamageReduction);
}

void UCompanionAttributeSetBase::OnRep_KnockBackResistance(const FGameplayAttributeData& OldKnockBackResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, KnockBackResistance, OldKnockBackResistance);
}

void UCompanionAttributeSetBase::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCompanionAttributeSetBase, MovementSpeed, OldMovementSpeed);
}
