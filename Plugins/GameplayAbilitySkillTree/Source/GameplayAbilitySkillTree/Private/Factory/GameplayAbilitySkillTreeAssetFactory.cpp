// Fill out your copyright notice in the Description page of Project Settings.



#include "Factory/GameplayAbilitySkillTreeAssetFactory.h"

#include "GameplayAbilitySkillTreeAsset.h"

UGameplayAbilitySkillTreeAssetFactory::UGameplayAbilitySkillTreeAssetFactory(
	const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
	SupportedClass = UGameplayAbilitySkillTreeAsset::StaticClass();
}

UObject* UGameplayAbilitySkillTreeAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
	EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UGameplayAbilitySkillTreeAsset* AbilitySkillTreeAsset = NewObject<UGameplayAbilitySkillTreeAsset>(InParent, InName, Flags);
	return AbilitySkillTreeAsset;
}

bool UGameplayAbilitySkillTreeAssetFactory::CanCreateNew() const
{
	return true;
}
