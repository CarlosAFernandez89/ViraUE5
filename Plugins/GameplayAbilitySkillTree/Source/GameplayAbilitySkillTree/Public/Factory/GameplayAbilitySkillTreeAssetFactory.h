// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GameplayAbilitySkillTreeAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSKILLTREE_API UGameplayAbilitySkillTreeAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGameplayAbilitySkillTreeAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
