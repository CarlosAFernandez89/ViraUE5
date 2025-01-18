// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectBlueprintFunctionLibrary.h"

#include "GameplayEffect.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"

float UGameplayEffectBlueprintFunctionLibrary::GetModifierMagnitudeFromClass(const UObject* WorldContextObject,
                                                                             const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const int32 ModifierIndex)
{
	if (!GameplayEffectClass || !WorldContextObject)
	{
		return -1.f;
	}

	const UGameplayEffect* GameplayEffectCDO = GameplayEffectClass.GetDefaultObject();
	float CalculatedMagnitude = 0.f;

	const FGameplayModifierInfo& ModifierInfo = GameplayEffectCDO->Modifiers[ModifierIndex];
	const FGameplayEffectModifierMagnitude& Magnitude = ModifierInfo.ModifierMagnitude;

	switch (Magnitude.GetMagnitudeCalculationType())
	{
	case EGameplayEffectMagnitudeCalculation::ScalableFloat:
		{
			Magnitude.GetStaticMagnitudeIfPossible(1, CalculatedMagnitude);
			break;
		}
	case EGameplayEffectMagnitudeCalculation::AttributeBased:
		{
			// 5.b. Attribute Based
			TArray<FGameplayEffectAttributeCaptureDefinition> AttributeCaptureDefinitions;
			Magnitude.GetAttributeCaptureDefinitions(AttributeCaptureDefinitions);

			for (const FGameplayEffectAttributeCaptureDefinition& AttributeCaptureDefinition : AttributeCaptureDefinitions)
			{
				FGameplayEffectSpec EffectSpec;
				EffectSpec.AddModifiedAttribute(AttributeCaptureDefinition.AttributeToCapture);
				Magnitude.AttemptCalculateMagnitude(EffectSpec, CalculatedMagnitude);
			}
				
			break;
		}
	case EGameplayEffectMagnitudeCalculation::CustomCalculationClass:
		{
			TSubclassOf<UGameplayModMagnitudeCalculation> CustomClass = Magnitude.GetCustomMagnitudeCalculationClass();
			break;
		}
	case EGameplayEffectMagnitudeCalculation::SetByCaller:
		{
			FSetByCallerFloat SetByCallerFloat = Magnitude.GetSetByCallerFloat();
			break;
		}
	}
	return CalculatedMagnitude;
}

float UGameplayEffectBlueprintFunctionLibrary::GetExecutionsMagnitudeFromClass(const UObject* WorldContextObject,
	const TSubclassOf<UGameplayEffect>& GameplayEffectClass,const int32 ExecutionIndex, const int32 ModifierIndex)
{
	if (!GameplayEffectClass || !WorldContextObject)
	{
		return -1.f;
	}

	const UGameplayEffect* GameplayEffectCDO = GameplayEffectClass.GetDefaultObject();
	
	float CalculatedMagnitude = 0.f;
	
	const FGameplayEffectExecutionDefinition& ExecutionDefinition = GameplayEffectCDO->Executions[ExecutionIndex];
	const FGameplayEffectExecutionScopedModifierInfo Info = ExecutionDefinition.CalculationModifiers[ModifierIndex];
	const FGameplayEffectModifierMagnitude& Magnitude = Info.ModifierMagnitude;

	switch (ExecutionDefinition.CalculationModifiers[ModifierIndex].ModifierMagnitude.GetMagnitudeCalculationType())
	{
	case EGameplayEffectMagnitudeCalculation::ScalableFloat:
		{
			Magnitude.GetStaticMagnitudeIfPossible(1, CalculatedMagnitude);
			break;
		}
	case EGameplayEffectMagnitudeCalculation::AttributeBased:
		{
			// 5.b. Attribute Based
			TArray<FGameplayEffectAttributeCaptureDefinition> AttributeCaptureDefinitions;
			Magnitude.GetAttributeCaptureDefinitions(AttributeCaptureDefinitions);

			for (const FGameplayEffectAttributeCaptureDefinition& AttributeCaptureDefinition : AttributeCaptureDefinitions)
			{
				FGameplayEffectSpec EffectSpec;
				EffectSpec.AddModifiedAttribute(AttributeCaptureDefinition.AttributeToCapture);
				Magnitude.AttemptCalculateMagnitude(EffectSpec, CalculatedMagnitude);
			}
			break;
		}
	case EGameplayEffectMagnitudeCalculation::CustomCalculationClass:
		{
			TSubclassOf<UGameplayModMagnitudeCalculation> CustomClass = Magnitude.GetCustomMagnitudeCalculationClass();

			break;
		}
	case EGameplayEffectMagnitudeCalculation::SetByCaller:
		{
			FSetByCallerFloat SetByCallerFloat = Magnitude.GetSetByCallerFloat();
			break;
		}
	}

	return CalculatedMagnitude;
}

TArray<float> UGameplayEffectBlueprintFunctionLibrary::GetDamageValueForLevels(const UObject* WorldContextObject,
	const FVyraGameplayAbilityInfo& AbilityInfo)
{
	TArray<float> DamageValueForLevels;
	const float BaseMagnitude = GetExecutionsMagnitudeFromClass(WorldContextObject, AbilityInfo.DamageEffect);

	if (UCurveTable* CurveTable = AbilityInfo.DamageCurveTable)
	{
		const FRealCurve* RealCurve = CurveTable->GetCurves()[0].CurveToEdit;
		for (int i = 0; i < RealCurve->GetNumKeys(); ++i)
		{
			DamageValueForLevels.Add(BaseMagnitude * RealCurve->Eval(i + 1));
		}
	}

	return DamageValueForLevels;
}
