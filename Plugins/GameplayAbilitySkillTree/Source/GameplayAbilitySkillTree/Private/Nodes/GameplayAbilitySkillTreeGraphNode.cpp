// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/GameplayAbilitySkillTreeGraphNode.h"

void UGameplayAbilitySkillTreeGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Custom Node Actions")));
	
	UGameplayAbilitySkillTreeGraphNode* Node = const_cast<UGameplayAbilitySkillTreeGraphNode*>(this);
	Section.AddMenuEntry(
		TEXT("AddPinEntry"),
		FText::FromString(TEXT("Add Pin")),
		FText::FromString(TEXT("Creates a new pin")),
		FSlateIcon(TEXT("GameplayAbilitySkillTreeStyle"), TEXT("GameplayAbilitySkillTree.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] () {
				Node->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("AnotherOutput"));
				Node->GetGraph()->NotifyGraphChanged();
				Node->GetGraph()->Modify();
			}
		))
	);

	Section.AddMenuEntry(
		TEXT("DeletePinEntry"),
		FText::FromString(TEXT("Delete Pin")),
		FText::FromString(TEXT("Deletes the last pin")),
		FSlateIcon(TEXT("GameplayAbilitySkillTreeStyle"), TEXT("GameplayAbilitySkillTree.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] () {
				UEdGraphPin* pin = Node->GetPinAt(Node->Pins.Num() - 1);
				if (pin->Direction != EEdGraphPinDirection::EGPD_Input) {
					Node->RemovePin(pin);

					Node->GetGraph()->NotifyGraphChanged();
					Node->GetGraph()->Modify();
				}
			}
		))
	);

	Section.AddMenuEntry(
		TEXT("DeleteEntry"),
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Deletes the node")),
		FSlateIcon(TEXT("GameplayAbilitySkillTreeStyle"), TEXT("GameplayAbilitySkillTree.NodeDeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] () {
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}

UEdGraphPin* UGameplayAbilitySkillTreeGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Name)
{
	FName Category = (Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName Subcategory = TEXT("CustomPin");

	UEdGraphPin* pin = CreatePin(
		Direction,
		Category,
		Name
	);
	pin->PinType.PinSubCategory = Subcategory;

	return pin;
}
