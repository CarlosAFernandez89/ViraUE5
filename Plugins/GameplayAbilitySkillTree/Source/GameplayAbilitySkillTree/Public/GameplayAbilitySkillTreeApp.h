// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

/**
 * 
 */
class FGameplayAbilitySkillTreeApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook 
{
	
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);

	class UGameplayAbilitySkillTreeAsset* GetWorkingSkillTree() { return WorkingSkillTree; }
	class UEdGraph* GetWorkingGraph() { return WorkingSkillTreeGraph; }
public:
	virtual FName GetToolkitFName() const override {return FName("GameplayAbilitySkillTreeApp"); };
	virtual FText GetBaseToolkitName() const override { return FText::FromString("GameplayAbilitySkillTreeApp"); };
	virtual FString GetWorldCentricTabPrefix() const override { return FString("GameplayAbilitySkillTreeApp"); };
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }
	virtual FString GetDocumentationLink() const override { return TEXT(""); }
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override { };
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override { };

	virtual void OnClose() override;
	void OnGraphChanged(const FEdGraphEditAction& EditAction);

protected:
	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();
	
private:
	class UGameplayAbilitySkillTreeAsset* WorkingSkillTree = nullptr;
	class UEdGraph* WorkingSkillTreeGraph = nullptr;
	
	FDelegateHandle GraphChangeListenerHandle;
};
