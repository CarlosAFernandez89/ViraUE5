// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/**
 * 
 */
class GAMEPLAYABILITYSKILLTREE_API FGameplayAbilitySkillTreeAppMode : public FApplicationMode
{
public:
	FGameplayAbilitySkillTreeAppMode(TSharedPtr<class FGameplayAbilitySkillTreeApp> InApp);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class FGameplayAbilitySkillTreeApp> App;
	FWorkflowAllowedTabSet TabSet;
};
