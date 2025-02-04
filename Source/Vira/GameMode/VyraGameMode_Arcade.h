// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularGameMode.h"
#include "VyraGameMode_Arcade.generated.h"

class UVyraWaveManagerComponent;
class UPowerUpComponent;
/**
 * 
 */
UCLASS()
class VIRA_API AVyraGameMode_Arcade : public AGSCModularGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(visibleAnywhere, BlueprintReadWrite,Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPowerUpComponent> PowerUpComponent;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite,Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVyraWaveManagerComponent> WaveManagerComponent;
public:
	AVyraGameMode_Arcade();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPowerUpComponent* GetPowerUpComponent() const { return PowerUpComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UVyraWaveManagerComponent* GetWaveManagerComponent() const { return WaveManagerComponent; }
};
