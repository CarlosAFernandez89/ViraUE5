// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_GameSettings.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_GameSettings : public USaveGame
{
	GENERATED_BODY()

public:
	// Settings to Save
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	bool bShowDamageNumbers = true;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	bool bEnableCameraShake = true;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	bool bShowEnemyHealthBars = true;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	bool bShowBlood = true;
};
