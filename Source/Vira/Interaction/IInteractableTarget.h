﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InteractionOption.h"
#include "IInteractableTarget.generated.h"

struct FInteractionQuery;

/**  */
class FInteractionOptionBuilder
{
public:
	FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FInteractionOption& Option)
	{
		FInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<IInteractableTarget> Scope;
	TArray<FInteractionOption>& Options;
};

/**  */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IInteractableTarget
{
	GENERATED_BODY()

public:
	/**  */
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) = 0;

	/**  */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) { }

};

UINTERFACE(BlueprintType)
class UInteractionInfoAndActions : public UInterface
{
	GENERATED_BODY()
};

class IInteractionInfoAndActions
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	FInteractionOption GetInteractionOption();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	void SetInteractionText(const FText& InteractionText);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	void OnInteractEvent(FGameplayEventData Payload);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	void EnableInteractUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	void DisableInteractUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="InteractionInfoAndActions")
	void SetActive_InteractableActor(bool bActive = true);
	
};
