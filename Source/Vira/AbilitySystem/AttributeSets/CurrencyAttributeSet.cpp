// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrencyAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCurrencyAttributeSet::UCurrencyAttributeSet()
{
}

void UCurrencyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetSoulsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,UE_BIG_NUMBER);
	}
	else if(Attribute == GetPickUpRadiusAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,2000.f);
	}
	else if(Attribute == GetItemDropRateIncreaseAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,10.f);
	}
}

void UCurrencyAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGSCAttributeSetExecutionData ExecutionData;
	GetExecutionDataFromMod(Data, ExecutionData);
}

void UCurrencyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCurrencyAttributeSet, Souls, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCurrencyAttributeSet, PickUpRadius, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCurrencyAttributeSet, ItemDropRateIncrease, COND_None, REPNOTIFY_Always);


}

void UCurrencyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetSoulsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,UE_BIG_NUMBER);
	}
	else if(Attribute == GetPickUpRadiusAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,2000.f);
	}
	else if(Attribute == GetItemDropRateIncreaseAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f,10.f);
	}
}

void UCurrencyAttributeSet::OnRep_Souls(const FGameplayAttributeData& OldSouls)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCurrencyAttributeSet, Souls, OldSouls);
}

void UCurrencyAttributeSet::OnRep_PickUpRadius(const FGameplayAttributeData& OldPickUpRadius)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCurrencyAttributeSet, PickUpRadius, OldPickUpRadius);
}

void UCurrencyAttributeSet::OnRep_ItemDropRateIncrease(const FGameplayAttributeData& OldItemDropRateIncrease)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCurrencyAttributeSet, ItemDropRateIncrease, OldItemDropRateIncrease);
}
