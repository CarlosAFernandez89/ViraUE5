// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySkillTreeRuntimeGraph.h"
#include "UObject/Object.h"
#include "GameplayAbilitySkillTreeAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEPLAYABILITYSKILLTREERUNTIME_API UGameplayAbilitySkillTreeAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString SomeData = TEXT("DefaultData");

	UPROPERTY(EditAnywhere)
	int32 SomeNumber = 0;

	UPROPERTY(EditAnywhere)
	bool SomeBool = false;

	UPROPERTY()
	UGameplayAbilitySkillTreeRuntimeGraph* Graph = nullptr;
};
