﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraDamageExecutionCalculation.h"

#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);

	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeDamageMultiplier);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DamageReduction, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, BaseDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, CriticalStrikeChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, CriticalStrikeDamageMultiplier, Source, true);
	}
};

static FDamageStatics DamageStatics()
{
	static FDamageStatics statics;
	return statics;
}

UVyraDamageExecutionCalculation::UVyraDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);

	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeDamageMultiplierDef);
}

void UVyraDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float LocalBaseDamage = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParams, LocalBaseDamage);

	float LocalCriticalStrikeChance = 0.05f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeChanceDef, EvaluateParams, LocalCriticalStrikeChance);
	
	float LocalCriticalStrikeDamageMultiplier = 0.05f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeDamageMultiplierDef, EvaluateParams, LocalCriticalStrikeDamageMultiplier);

	float LocalDamageReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvaluateParams, LocalDamageReduction);

	if(LocalBaseDamage < 0)
	{
		LocalBaseDamage = 0.f;
	}

	if(FMath::RandRange(0.f, 1.f) <= LocalCriticalStrikeChance)
	{
		LocalBaseDamage *= LocalCriticalStrikeDamageMultiplier;
	}

	LocalBaseDamage *= (1.f - LocalDamageReduction);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -LocalBaseDamage));
}
