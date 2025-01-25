// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "VyraDamageExecutionCalculation_Companion.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraDamageExecutionCalculation_Companion : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UVyraDamageExecutionCalculation_Companion();
	
protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
