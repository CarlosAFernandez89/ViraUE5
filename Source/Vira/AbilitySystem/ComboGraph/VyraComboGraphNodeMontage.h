// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/ComboGraphNodeMontage.h"

#include "VyraComboGraphNodeMontage.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraComboGraphNodeMontage : public UComboGraphNodeMontage
{
	GENERATED_BODY()

	protected:
	UVyraComboGraphNodeMontage();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	
	public:

	UFUNCTION(BlueprintCallable, Category = "Vyra|ComboGraph")
	void SetComboGraphMontagePlayRate(const float InPlayRate){ MontagePlayRate = InPlayRate ; };
};
