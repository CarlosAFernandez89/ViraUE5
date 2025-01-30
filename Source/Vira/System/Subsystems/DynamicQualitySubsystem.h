// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DynamicQualitySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UDynamicQualitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Dynamic Quality")
	void SetTargetFPS(int32 NewTargetFPS);


	/* 
	 * We gather FPS info while this is off, but we don't actually change the FPS.
	 * This allows us to quickly get results when toggling the option.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dynamic Quality")
	void SetEnableDynamicQuality(bool bEnable);

protected:

	bool bEnableDynamicQuality = false;
	int32 TargetFPS = 120;
	TArray<float> FPSBuffer;
	int32 MaxSamples = 30; // 15-second window at 0.5s intervals
	float SampleInterval = 0.5f;
	float AdjustmentCooldown = 10.0f;
	float TimeSinceLastAdjustment = 0.0f;

	TArray<FName> LoweringPriorityOrder = {
		FName("ShadowQuality"),
		FName("PostProcessQuality"),
		FName("AntiAliasingQuality"),
		FName("TextureQuality"),
		FName("EffectsQuality"),
		FName("ViewDistance") // Top Down Game, we don't really care about this
	};
	TArray<FName> IncreasingPriorityOrder;

	TMap<FName, int32> InitialScalabilityLevels;

	FTimerHandle FPSSampleTimerHandle;
	FTimerHandle AdjustmentCheckTimerHandle;

	void SampleFPS();
	void CheckAndAdjustQuality();
	void AdjustQuality(bool bIncreaseQuality);
	float GetAverageFPS() const;

	void SaveSettings();
	void LoadSettings();

private:
	
	static int32 GetQualityLevelForGroup(const UGameUserSettings* Settings, FName GroupName);
	static void SetQualityLevelForGroup(const UGameUserSettings* Settings, FName GroupName, const int32 NewLevel);

public:
	UDynamicQualitySubsystem();
};
