// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraWidget_PowerUp_Slot.h"
#include "GameFramework/GameplayMessageTypes2.h"
#include "Vira/GameMode/Components/PowerUpComponent.h"
#include "Vira/UI/Foundation/VyraActivatableWidget.h"
#include "VyraWidget_PowerUp_Panel.generated.h"

USTRUCT(BlueprintType)
struct FPowerUpSlot_Selected
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UVyraWidget_PowerUp_Slot* SlotReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPowerUpData PowerUpData;
};

/**
 * 
 */

UCLASS()
class VIRA_API UVyraWidget_PowerUp_Panel : public UVyraActivatableWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void AddPowerUpToArray(const FPowerUpData InPowerUp);

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	FORCEINLINE TArray<FPowerUpData> GetPowerUpArray() { return PowerUpsData; };

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PowerUp, meta = (ExposeOnSpawn = true))
	TArray<FPowerUpData> PowerUpsData;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PowerUp")
	UVyraWidget_PowerUp_Slot* SelectedSlot;
};
