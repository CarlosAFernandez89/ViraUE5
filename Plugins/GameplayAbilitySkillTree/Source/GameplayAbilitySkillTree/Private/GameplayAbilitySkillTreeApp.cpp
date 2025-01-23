// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySkillTreeApp.h"
#include "GameplayAbilitySkillTreeAppMode.h"
#include "GameplayAbilitySkillTreeAsset.h"
#include "GameplayAbilitySkillTreeRuntimeGraph.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Nodes/GameplayAbilitySkillTreeGraphNode.h"
#include "Schema/GameplayAbilitySkillTreeSchema.h"


void FGameplayAbilitySkillTreeApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FGameplayAbilitySkillTreeApp::InitEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	TArray<UObject*> Objects;
	Objects.Add(InObject);

	WorkingSkillTree = Cast<UGameplayAbilitySkillTreeAsset>(InObject);

	WorkingSkillTreeGraph = FBlueprintEditorUtils::CreateNewGraph(
		WorkingSkillTree,
		NAME_None,
		UEdGraph::StaticClass(),
		UGameplayAbilitySkillTreeSchema::StaticClass());
	
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		FName("GameplayAbilitySkillTree"),
		FTabManager::FLayout::NullLayout,
		true, //Create standalone menu
		true, //Create default toolbar
		Objects
		);

	AddApplicationMode(
		TEXT("GameplayAbilitySkillTreeAppMode"),
		MakeShareable(new FGameplayAbilitySkillTreeAppMode(SharedThis(this)))
		);

	SetCurrentMode(TEXT("GameplayAbilitySkillTreeAppMode"));

	
	// Load the UI from the asset
	UpdateEditorGraphFromWorkingAsset();

	GraphChangeListenerHandle = WorkingSkillTreeGraph->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateSP(this, &FGameplayAbilitySkillTreeApp::OnGraphChanged)
	);
}


void FGameplayAbilitySkillTreeApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingSkillTreeGraph->RemoveOnGraphChangedHandler(GraphChangeListenerHandle);
	FAssetEditorToolkit::OnClose();
}

void FGameplayAbilitySkillTreeApp::OnGraphChanged(const FEdGraphEditAction& EditAction)
{
	UpdateWorkingAssetFromGraph();
}

void FGameplayAbilitySkillTreeApp::UpdateWorkingAssetFromGraph()
{
	if (WorkingSkillTree == nullptr || WorkingSkillTreeGraph == nullptr) return;

	// Update the state we need into our saveable format
	UGameplayAbilitySkillTreeRuntimeGraph* RuntimeGraph = NewObject<UGameplayAbilitySkillTreeRuntimeGraph>(WorkingSkillTree);
	WorkingSkillTree->Graph = RuntimeGraph;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UGameplayAbilitySkillTreeRuntimePin*> IDToPinMap;

	// First create all the nodes/pins and record the connections
	for (UEdGraphNode* UINode : WorkingSkillTreeGraph->Nodes)
	{
		UGameplayAbilitySkillTreeRuntimeNode* RuntimeNode = NewObject<UGameplayAbilitySkillTreeRuntimeNode>(RuntimeGraph);
		RuntimeNode->Position = FVector2D(UINode->NodePosX, UINode->NodePosY);

		for (UEdGraphPin* UIPin : UINode->Pins)
		{
			UGameplayAbilitySkillTreeRuntimePin* RuntimePin = NewObject<UGameplayAbilitySkillTreeRuntimePin>(RuntimeNode);
			RuntimePin->PinName = UIPin->PinName;
			RuntimePin->PinId = UIPin->PinId;

			// Only record the output side of the connection since this is a directed graph
			if (UIPin->HasAnyConnections() && UIPin->Direction == EEdGraphPinDirection::EGPD_Output)
			{
				// Only 1 connection is allowed so just take the first one
				std::pair<FGuid, FGuid> Connection = std::make_pair(UIPin->PinId, UIPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}
            
			IDToPinMap.Add(UIPin->PinId, RuntimePin);
			if (UIPin->Direction == EEdGraphPinDirection::EGPD_Input)
			{
				RuntimeNode->InputPin = RuntimePin;
			}
			else
			{
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}

		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	// Now make all the connections
	for (std::pair<FGuid, FGuid> Connection : Connections)
	{
		UGameplayAbilitySkillTreeRuntimePin* Pin1 = IDToPinMap[Connection.first];
		UGameplayAbilitySkillTreeRuntimePin* Pin2 = IDToPinMap[Connection.second];
		Pin1->Connection = Pin2;
	}
}

void FGameplayAbilitySkillTreeApp::UpdateEditorGraphFromWorkingAsset()
{
	if (WorkingSkillTree->Graph == nullptr)
	{
		return;
	}

	// Create all the nodes/pins first
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IDToPinMap;
	for (UGameplayAbilitySkillTreeRuntimeNode* RuntimeNode : WorkingSkillTree->Graph->Nodes)
	{
		UGameplayAbilitySkillTreeGraphNode* NewNode = NewObject<UGameplayAbilitySkillTreeGraphNode>(WorkingSkillTreeGraph);
		NewNode->CreateNewGuid();
		NewNode->NodePosX = RuntimeNode->Position.X;
		NewNode->NodePosY = RuntimeNode->Position.Y;

		if (RuntimeNode->InputPin != nullptr)
		{
			UGameplayAbilitySkillTreeRuntimePin* Pin = RuntimeNode->InputPin;
			UEdGraphPin* UIPin = NewNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, Pin->PinName);
			UIPin->PinId = Pin->PinId;

			if (Pin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
			IDToPinMap.Add(Pin->PinId, UIPin);
		}

		for (UGameplayAbilitySkillTreeRuntimePin* Pin : RuntimeNode->OutputPins)
		{
			UEdGraphPin* UIPin = NewNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, Pin->PinName);
			UIPin->PinId = Pin->PinId;
            
			if (Pin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
			IDToPinMap.Add(Pin->PinId, UIPin);
		}

		WorkingSkillTreeGraph->AddNode(NewNode, true, true);
	}

	for (std::pair<FGuid, FGuid> connection : Connections)
	{
		UEdGraphPin* FromPin = IDToPinMap[connection.first];
		UEdGraphPin* ToPin = IDToPinMap[connection.second];
		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}
