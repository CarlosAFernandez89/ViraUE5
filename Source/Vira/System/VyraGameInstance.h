// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/SaveGame.h"
#include "VyraGameInstance.generated.h"

class FWorldDelegates;

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVyraGameInstance();

	virtual void Init() override; // Override the Init function

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	void UpdateSaveSlotName(const FString& NewSaveSlotName);

	void SaveCurrentPlayerData();

	void LoadCurrentPlayerData();
	void AsyncLoadPlayerDataComplete(const FString& SlotName, int UserIndex, USaveGame* LoadedGameData);

	void SaveCurrentLevel(const FString& SlotName);
	
	void LoadCurrentLevel(const FString& SlotName);
	
	void OnLevelLoaded(const FActorsInitializedParams& Params);

private:
	
	FString CurrentLevelSaveSlot;

public:

	// We use this to manage which save slot we loaded at the start of the game.
	UPROPERTY()
	FString CurrentSlotName;
};
