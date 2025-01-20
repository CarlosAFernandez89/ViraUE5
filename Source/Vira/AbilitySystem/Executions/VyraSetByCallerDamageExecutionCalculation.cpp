// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraSetByCallerDamageExecutionCalculation.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Perception/AISense_Damage.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "Vira/Player/Components/FloatingCombatText.h"

struct FSetByCallerDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);

	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeDamageMultiplier);

	FSetByCallerDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DamageReduction, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, BaseDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, CriticalStrikeChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, CriticalStrikeDamageMultiplier, Source, true);
	}
};

static FSetByCallerDamageStatics SetByCallerDamageStatics()
{
	static FSetByCallerDamageStatics statics;
	return statics;
}

UVyraSetByCallerDamageExecutionCalculation::UVyraSetByCallerDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(SetByCallerDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(SetByCallerDamageStatics().DamageReductionDef);

	RelevantAttributesToCapture.Add(SetByCallerDamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(SetByCallerDamageStatics().CriticalStrikeChanceDef);
	RelevantAttributesToCapture.Add(SetByCallerDamageStatics().CriticalStrikeDamageMultiplierDef);
}

void UVyraSetByCallerDamageExecutionCalculation::Execute_Implementation(
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


	float LocalBaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SetByCallerDamageStatics().BaseDamageDef, EvaluateParams, LocalBaseDamage);

	float LocalBaseDamageMultiplier = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Damage"), false, -1.), 0.f);

	float LocalCriticalStrikeChance = 0.05f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SetByCallerDamageStatics().CriticalStrikeChanceDef, EvaluateParams, LocalCriticalStrikeChance);
	
	float LocalCriticalStrikeDamageMultiplier = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SetByCallerDamageStatics().CriticalStrikeDamageMultiplierDef, EvaluateParams, LocalCriticalStrikeDamageMultiplier);

	float LocalDamageReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SetByCallerDamageStatics().DamageReductionDef, EvaluateParams, LocalDamageReduction);

	if(LocalBaseDamage < 0)
	{
		LocalBaseDamage = 0.f;
	}

	LocalBaseDamage *= LocalBaseDamageMultiplier;

	bool bCriticalHit = false;
	if(FMath::RandRange(0.f, 1.f) <= LocalCriticalStrikeChance)
	{
		LocalBaseDamage *= LocalCriticalStrikeDamageMultiplier;
		bCriticalHit = true;
	}

	LocalBaseDamage *= (1.f - LocalDamageReduction);

	UAISense_Damage::ReportDamageEvent(
		SourceActor,
		TargetActor,
		SourceActor,
		LocalBaseDamage,
		SourceActor->GetActorLocation(),
		FVector(0, 0, 0));

	if (SourceActor->IsA(AVyraPlayerStateCharacter::StaticClass()))
	{
		if (AVyraPlayerStateCharacter* PC = Cast<AVyraPlayerStateCharacter>(SourceActor))
		{
			PC->SpawnDamageText(TargetActor, LocalBaseDamage, bCriticalHit);
		}
	}
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(SetByCallerDamageStatics().HealthProperty, EGameplayModOp::Additive, -LocalBaseDamage));

}
