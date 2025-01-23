// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "GameplayAbilitySkillTreeSchema.generated.h"


USTRUCT(BlueprintType)
struct FNewGameplayAbilitySkillTreeNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FNewGameplayAbilitySkillTreeNodeAction() {}
	FNewGameplayAbilitySkillTreeNodeAction(FText InNodeCategory, FText InMenuDescription, FText InTooltip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDescription, InTooltip, InGrouping)
	{
		
	}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	
};
/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSKILLTREE_API UGameplayAbilitySkillTreeSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;
};
