// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_GameInstanceInfo.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_GameInstanceInfo : public USaveGame
{
	GENERATED_BODY()

public:
	UVyraSaveGame_GameInstanceInfo();

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
 
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY()
	int32 TotalSaveSlotsCreated;
	
	UPROPERTY()
	TArray<FString> AllSaveSlots;

	UPROPERTY()
	FString LastSaveSlotLoaded;
};
