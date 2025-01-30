// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_DynamicQuality.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_DynamicQuality : public USaveGame
{
	GENERATED_BODY()

public:
	// Settings to Save
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	bool bEnableDynamicQuality = false;
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	int32 TargetFPS = 120;
};
