// Fill out your copyright notice in the Description page of Project Settings.


#include "Factory/GameplayAbilitySkillTreeTabFactory.h"
#include "GameplayAbilitySkillTreeApp.h"
#include "GameplayAbilitySkillTreeAsset.h"

FGameplayAbilitySkillTreeTabFactory::FGameplayAbilitySkillTreeTabFactory(
	TSharedPtr<class FGameplayAbilitySkillTreeApp> InApp)
		: FWorkflowTabFactory(FName("Graph"), InApp)
{
	App = InApp;
	TabLabel = FText::FromString("Graph");
	ViewMenuDescription = FText::FromString("View Menu");
	ViewMenuTooltip = FText::FromString("View Menu Tooltip");
}

TSharedRef<SWidget> FGameplayAbilitySkillTreeTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<class FGameplayAbilitySkillTreeApp> TempApp = App.Pin();
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			SNew(SGraphEditor)
				.IsEditable(true)
				.GraphToEdit(TempApp->GetWorkingGraph())
		];
}

FText FGameplayAbilitySkillTreeTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A primary view for doing primary things."));
}



// ========================================================================================================
// Details Tabs Factory

FGameplayAbilitySkillTreeDetailsTabFactory::FGameplayAbilitySkillTreeDetailsTabFactory(
	TSharedPtr<class FGameplayAbilitySkillTreeApp> InApp)
		: FWorkflowTabFactory(FName("PropertiesTab"), InApp)
{
	App = InApp;
	TabLabel = FText::FromString("Properties");
	ViewMenuDescription = FText::FromString("View Menu");
	ViewMenuTooltip = FText::FromString("View Menu Tooltip");
}


TSharedRef<SWidget> FGameplayAbilitySkillTreeDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<class FGameplayAbilitySkillTreeApp> TempApp = App.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = false;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(TempApp->GetWorkingSkillTree());

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

FText FGameplayAbilitySkillTreeDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString("Properties for current asset.");
}
