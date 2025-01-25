// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraDamageExecutionCalculation_Companion.h"

#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Perception/AISense_Damage.h"
#include "Vira/AbilitySystem/AttributeSets/Companions/CompanionAttributeSetBase.h"
#include "Vira/AbilitySystem/Components/Companions/VyraCompanionPawn_WithASC.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"


struct FCompanionDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);

	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeDamageMultiplier);

	FCompanionDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSCAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCompanionAttributeSetBase, DamageReduction, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCompanionAttributeSetBase, BaseDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCompanionAttributeSetBase, CriticalStrikeChance, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCompanionAttributeSetBase, CriticalStrikeDamageMultiplier, Source, true);
	}
};

static FCompanionDamageStatics DamageStatics()
{
	static FCompanionDamageStatics statics;
	return statics;
}

UVyraDamageExecutionCalculation_Companion::UVyraDamageExecutionCalculation_Companion()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);

	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeDamageMultiplierDef);
}

void UVyraDamageExecutionCalculation_Companion::Execute_Implementation(
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

	//This is used to get the ScalableFloatMagnitude from the execution
	//It can use a CurveTable to scale the damage based on the effect level.
	float TotalCalculatedMagnitudes = 0.f;
	for (const FGameplayEffectExecutionDefinition& ExecutionDefinition : Spec.Def->Executions)
	{
		for (auto Modifier : ExecutionDefinition.CalculationModifiers)
		{
			float CalculatedMagnitude = 0.0f;
			Modifier.ModifierMagnitude.AttemptCalculateMagnitude(Spec, CalculatedMagnitude);
			TotalCalculatedMagnitudes += CalculatedMagnitude;
		}
	}

	// Add it to the base character damage.
	LocalBaseDamage += TotalCalculatedMagnitudes;
	
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

	if (SourceActor->IsA(AVyraCompanionPawn_WithASC::StaticClass()))
	{
		if (AActor* OwnerPawn = SourceActor->GetOwner())
		{
			if (AVyraPlayerStateCharacter* PC = Cast<AVyraPlayerStateCharacter>(OwnerPawn))
			{
				PC->SpawnDamageText(TargetActor, LocalBaseDamage, bCriticalHit);
			}
		}
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -LocalBaseDamage));
}
