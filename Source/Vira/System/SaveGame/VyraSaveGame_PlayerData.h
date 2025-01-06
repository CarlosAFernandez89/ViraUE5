// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSaveState.h"
#include "VyraSaveGame_Charms.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_PlayerData.generated.h"


USTRUCT(BlueprintType)
struct FSaveGamePlayerData_Currency
{
	GENERATED_BODY()

	UPROPERTY()
	float Souls = 0;

	UPROPERTY()
	float Gold = 0;

	FSaveGamePlayerData_Currency()
	{}

	FSaveGamePlayerData_Currency(const float InSouls, const float InGold)
	{
		Souls = InSouls;
		Gold = InGold;
	}
};


/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_PlayerData : public USaveGame
{
	GENERATED_BODY()


public:
	UVyraSaveGame_PlayerData();

	UFUNCTION(Category = "Vyra|SaveGame")
	void SaveAllPlayerData(const FString& SlotName, UObject* WorldContextObject);

	UFUNCTION(Category = "Vyra|SaveGame")
	void LoadAllPlayerData(UObject* WorldContextObject);

	UFUNCTION(Category = "Vyra|SaveGame")
	FSaveGamePlayerData_Currency GetCurrentPlayerCurrency(UObject* WorldContextObject);
	
	UFUNCTION(Category = "Vyra|SaveGame")
	FQuestSaveStateData GetCurrentQuestState(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	void UpdateCurrency(const FSaveGamePlayerData_Currency& Data);

	UFUNCTION(BlueprintCallable, Category = "Vyra|SaveGame")
	FSaveGamePlayerData_Currency GetLoadedCurrency();

	float GetTotalPlayTime() const {return TotalPlayTime;}
	void SetTotalPlayTime(float InTotalPlayTime) { TotalPlayTime = InTotalPlayTime;}

	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

private:

	// Charm Data
	UPROPERTY()
	FSaveGame_Charms AllCharms;

	//Questdata
	UPROPERTY()
	FQuestSaveStateData CurrentQuestSaveState;

	// Currency Data
	UPROPERTY()
	float Currency_Souls;
	UPROPERTY()
	float Currency_Gold;

	UPROPERTY()
	float TotalPlayTime;
};
