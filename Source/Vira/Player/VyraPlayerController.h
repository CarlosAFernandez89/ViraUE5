﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerController.h"
#include "VyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API AVyraPlayerController : public AGSCModularPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnWindowFocusChanged(bool bIsFocused);
};
