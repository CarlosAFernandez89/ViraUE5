// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VyraStringUtils.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraStringUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "Vyra|Utils", meta = (WorldContext = "WorldContextObject"))
	static FString StringArrayToString(UObject* WorldContextObject, TArray<FString> StringArray, FString StartingString = FString(""));

	UFUNCTION(BlueprintPure, Category = "Vyra|Utils", meta = (WorldContext = "WorldContextObject"))
	static FString DamageToShortStringFormater(UObject* WorldContextObject, float InFloat, bool bIncludeDecimals = true, int32 InDecimals = 2);

	UFUNCTION(BlueprintPure, Category = "Vyra|Utils", meta = (WorldContext = "WorldContextObject"))
	static FString DamageToLongStringFormater(UObject* WorldContextObject, float InFloat);
};
