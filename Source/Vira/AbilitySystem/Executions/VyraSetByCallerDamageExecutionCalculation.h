// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraDamageExecutionCalculation.h"
#include "VyraSetByCallerDamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraSetByCallerDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UVyraSetByCallerDamageExecutionCalculation();
protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
