// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VyraVerbMessageHelpers.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Vyra")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Vyra")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);
};
