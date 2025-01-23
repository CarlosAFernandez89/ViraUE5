// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/**
 * 
 */
class GAMEPLAYABILITYSKILLTREE_API FGameplayAbilitySkillTreeTabFactory : public FWorkflowTabFactory
{
	
public:
	FGameplayAbilitySkillTreeTabFactory(TSharedPtr<class FGameplayAbilitySkillTreeApp> InApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class FGameplayAbilitySkillTreeApp> App;	
};


class GAMEPLAYABILITYSKILLTREE_API FGameplayAbilitySkillTreeDetailsTabFactory : public FWorkflowTabFactory
{
	
public:
	FGameplayAbilitySkillTreeDetailsTabFactory(TSharedPtr<class FGameplayAbilitySkillTreeApp> InApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	
private:
	TWeakPtr<class FGameplayAbilitySkillTreeApp> App;	
};