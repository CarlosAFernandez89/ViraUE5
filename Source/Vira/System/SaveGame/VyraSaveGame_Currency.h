// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_Currency.generated.h"

USTRUCT(BlueprintType)
struct FSaveGame_Currency
{
	GENERATED_BODY()

	float Souls;
};

class UCurrencyAttributeSet;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_Currency : public USaveGame
{
	GENERATED_BODY()


public:

	UVyraSaveGame_Currency();
	
	UFUNCTION()
	void SaveCurrency(const FSaveGame_Currency Currency);

	UFUNCTION()
	FSaveGame_Currency LoadCurrency() const;

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
 
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

private:
	// Currency
	UPROPERTY()
	float Currency_Souls;
	
public:


};
