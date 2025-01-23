// Fill out your copyright notice in the Description page of Project Settings.


#include "Schema/GameplayAbilitySkillTreeSchema.h"

#include "Nodes/GameplayAbilitySkillTreeGraphNode.h"

UEdGraphNode* FNewGameplayAbilitySkillTreeNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                                    const FVector2D Location, bool bSelectNewNode)
{
	UGameplayAbilitySkillTreeGraphNode* Result = NewObject<UGameplayAbilitySkillTreeGraphNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;

	UEdGraphPin* InputPin = Result->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("SomeInput"));
	Result->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output1"));

	// If the from pin is not null, connect them
	if (FromPin != nullptr) {
		Result->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}

	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, true);

	return Result;
}

void UGameplayAbilitySkillTreeSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewGameplayAbilitySkillTreeNodeAction> Action(
		new FNewGameplayAbilitySkillTreeNodeAction
		(
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Node")),
			FText::FromString(TEXT("Make a new Node")),
			0

		));

	ContextMenuBuilder.AddAction(Action);
}

const FPinConnectionResponse UGameplayAbilitySkillTreeSchema::CanCreateConnection(const UEdGraphPin* a,
	const UEdGraphPin* b) const
{
	// Neither can be nullptr
	if (a == nullptr || b == nullptr) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
	}

	// Make sure the directions are different (no inputs to inputs or outputs to outputs)
	if (a->Direction == b->Direction) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
	}

	// Else allow it but break all connections
	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}
