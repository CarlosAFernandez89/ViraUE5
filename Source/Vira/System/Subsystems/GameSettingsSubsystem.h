// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettingsSubsystem.generated.h"

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

	// Getter and Setter for Damage Numbers
	UFUNCTION(BlueprintCallable)
	bool GetShowDamageNumbers() const { return bShowDamageNumbers; }
	UFUNCTION(BlueprintCallable)
	void SetShowDamageNumbers(bool bValue) { bShowDamageNumbers = bValue; }

	// Getter and Setter for Camera Shake
	UFUNCTION(BlueprintCallable)
	bool GetEnableCameraShake() const { return bEnableCameraShake; }
	UFUNCTION(BlueprintCallable)
	void SetEnableCameraShake(bool bValue) { bEnableCameraShake = bValue; }

	// Override Initialize for setup if needed
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();
};
