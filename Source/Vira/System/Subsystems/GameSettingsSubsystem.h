// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettingsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowEnemyHealthBarsUpdated, bool, bState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowDamageNumbersUpdated, bool, bState);

/**
 * 
 */
UCLASS()
class VIRA_API UGameSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Example settings
	bool bShowDamageNumbers = true;
	bool bEnableCameraShake = true;
	bool bShowEnemyHealthBars = true;
	bool bShowBlood = true;

	UPROPERTY(BlueprintAssignable, Category="GameSettings")
	FOnShowEnemyHealthBarsUpdated OnShowHealthBarsUpdated;
	UPROPERTY(BlueprintAssignable, Category="GameSettings")
	FOnShowDamageNumbersUpdated OnShowDamageNumbersUpdated;
	
	// Getter and Setter for Damage Numbers
	UFUNCTION(BlueprintCallable)
	bool GetShowDamageNumbers() const { return bShowDamageNumbers; }
	UFUNCTION(BlueprintCallable)
	void SetShowDamageNumbers(const bool bValue)
	{
		bShowDamageNumbers = bValue;
		OnShowDamageNumbersUpdated.Broadcast(bShowDamageNumbers);
	}

	// Getter and Setter for Camera Shake
	UFUNCTION(BlueprintCallable)
	bool GetEnableCameraShake() const { return bEnableCameraShake; }
	UFUNCTION(BlueprintCallable)
	void SetEnableCameraShake(const bool bValue) { bEnableCameraShake = bValue; }

	// Getter and Setter for Enemy HealthBars
	UFUNCTION(BlueprintCallable)
	bool GetShowEnemyHealthBars() const { return bShowEnemyHealthBars; }
	UFUNCTION(BlueprintCallable)
	void SetShowEnemyHealthBars(bool bValue);

	// Getter and Setter for Enemy HealthBars
	UFUNCTION(BlueprintCallable)
	bool GetShowBlood() const { return bShowBlood; }
	UFUNCTION(BlueprintCallable)
	void SetShowBlood(bool bValue);

	// Override Initialize for setup if needed
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();
};
