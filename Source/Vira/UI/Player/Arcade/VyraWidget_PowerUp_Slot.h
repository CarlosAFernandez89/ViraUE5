// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Vira/GameMode/Components/PowerUpComponent.h"
#include "VyraWidget_PowerUp_Slot.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraWidget_PowerUp_Slot : public UCommonUserWidget
{
	GENERATED_BODY()

	protected:

	UFUNCTION(BlueprintCallable, Category = "PowerUp|Slot")
	void Broadcast_OnSelectedMessage(const FGameplayTag GameplayMessageChannel);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PowerUp, meta = (ExposeOnSpawn = true))
	FPowerUpData PowerUpData;
};
