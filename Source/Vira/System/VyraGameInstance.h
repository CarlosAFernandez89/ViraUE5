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

	void LoadGameInstanceInfo();

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	void CreateNewSaveSlot();
	
	void UpdateGameInstanceSaveFile();

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	bool LoadSaveSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	bool DeleteSaveSlot(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	void UpdateSaveSlotName(const FString& NewSaveSlotName);

	void SaveCurrentPlayerData();

	void LoadCurrentPlayerData();
	void AsyncLoadPlayerDataComplete(const FString& SlotName, int UserIndex, USaveGame* LoadedGameData);

	void SaveCurrentLevel();
	
	void LoadCurrentLevel();
	
	void OnLevelLoaded(const FActorsInitializedParams& Params);

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	FString GetCurrentSaveSlotName();

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	TArray<FString> GetAllSaveSlots() const { return AllSaveSlots; };

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	FString GetLastSaveSlotLoaded() const {return LastSaveSlotLoaded;}

private:
	
	FString CurrentLevelSaveSlot;

public:

	// We use this to manage which save slot we loaded at the start of the game.
	UPROPERTY()
	FString CurrentSlotName;

	UPROPERTY()
	TArray<FString> AllSaveSlots;

	UPROPERTY()
	int32 TotalSaveSlotsCreated;

	UPROPERTY()
	FString LastSaveSlotLoaded;
};
