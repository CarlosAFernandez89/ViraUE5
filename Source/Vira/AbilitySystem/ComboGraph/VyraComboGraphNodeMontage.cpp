// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraComboGraphNodeMontage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"

#define LOCTEXT_NAMESPACE "ComboGraphNodeMontage"

UVyraComboGraphNodeMontage::UVyraComboGraphNodeMontage()
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("ComboGraphNodeMontage_ContextMenuName", "Vyra Montage Combo Node");
#endif
}

void UVyraComboGraphNodeMontage::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);

	bool bSuccess;
	const float AttackSpeed = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActor(), UCombatAttributeSet::GetAttackSpeedAttribute(), bSuccess);

	bSuccess ? SetComboGraphMontagePlayRate(AttackSpeed) : SetComboGraphMontagePlayRate(1.f);
}
