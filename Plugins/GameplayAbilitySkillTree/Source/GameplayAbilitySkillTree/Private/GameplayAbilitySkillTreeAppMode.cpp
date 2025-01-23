// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySkillTreeAppMode.h"

#include "GameplayAbilitySkillTreeApp.h"
#include "Factory/GameplayAbilitySkillTreeTabFactory.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

FGameplayAbilitySkillTreeAppMode::FGameplayAbilitySkillTreeAppMode(TSharedPtr<FGameplayAbilitySkillTreeApp> InApp)
	: FApplicationMode("GameplayAbilitySkillTreeAppMode")
{
	App = InApp;
	TabSet.RegisterFactory(MakeShareable(new FGameplayAbilitySkillTreeTabFactory(InApp)));
	TabSet.RegisterFactory(MakeShareable(new FGameplayAbilitySkillTreeDetailsTabFactory(InApp)));

	TabLayout = FTabManager::NewLayout("GameplayAbilitySkillTreeAppMode_Layout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.75f)
				->AddTab(FName(TEXT("Graph")), ETabState::OpenedTab)
			)
			->Split
			(
			FTabManager::NewStack()
					->SetSizeCoefficient(0.25f)
					->AddTab(FName(TEXT("PropertiesTab")), ETabState::OpenedTab)
			)
		)
	);
}

void FGameplayAbilitySkillTreeAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FGameplayAbilitySkillTreeApp> TempApp = App.Pin();
	TempApp->PushTabFactories(TabSet);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FGameplayAbilitySkillTreeAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FGameplayAbilitySkillTreeAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
