// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicQualitySubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/IConsoleManager.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Vira/System/SaveGame/VyraSaveGame_DynamicQuality.h"

int32 UDynamicQualitySubsystem::GetQualityLevelForGroup(const UGameUserSettings* Settings,
                                                        const FName GroupName)
{
    if (Settings)
    {
        if (GroupName == TEXT("ShadowQuality"))
        {
            return Settings->GetGameUserSettings()->GetShadowQuality();
        }
        else if (GroupName == TEXT("PostProcessQuality"))
        {
            return Settings->GetGameUserSettings()->GetShadowQuality();
        }
        else if (GroupName == TEXT("AntiAliasingQuality"))
        {
            return Settings->GetGameUserSettings()->GetShadowQuality();
        }
        else if (GroupName == TEXT("TextureQuality"))
        {
            return Settings->GetGameUserSettings()->GetShadowQuality();
        }
        else if (GroupName == TEXT("EffectsQuality"))
        {
            return Settings->GetGameUserSettings()->GetShadowQuality();
        }
        else if (GroupName == TEXT("ViewDistance"))
        {
            return Settings->GetGameUserSettings()->GetViewDistanceQuality();
        }
    }

   
    return Settings->GetGameUserSettings()->GetOverallScalabilityLevel();
}

void UDynamicQualitySubsystem::SetQualityLevelForGroup(const UGameUserSettings* Settings, FName GroupName,
    const int32 NewLevel)
{
    if (Settings)
    {
        if (GroupName == TEXT("ShadowQuality"))
        {
            Settings->GetGameUserSettings()->SetShadowQuality(NewLevel);
        }
        else if (GroupName == TEXT("PostProcessQuality"))
        {
            Settings->GetGameUserSettings()->SetPostProcessingQuality(NewLevel);
        }
        else if (GroupName == TEXT("AntiAliasingQuality"))
        {
            Settings->GetGameUserSettings()->SetAntiAliasingQuality(NewLevel);
        }
        else if (GroupName == TEXT("TextureQuality"))
        {
            Settings->GetGameUserSettings()->SetTextureQuality(NewLevel);
        }
        else if (GroupName == TEXT("EffectsQuality"))
        {
            Settings->GetGameUserSettings()->SetVisualEffectQuality(NewLevel);
        }
        else if (GroupName == TEXT("ViewDistance"))
        {
            Settings->GetGameUserSettings()->SetViewDistanceQuality(NewLevel);
        }
    }
}

UDynamicQualitySubsystem::UDynamicQualitySubsystem()
{
    IncreasingPriorityOrder = {
        FName("EffectsQuality"),
        FName("TextureQuality"),
        FName("AntiAliasingQuality"),
        FName("PostProcessQuality"),
        FName("ShadowQuality"),
        FName("ViewDistance") // Top Down Game, we don't really care about this
    };
}

void UDynamicQualitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    LoadSettings();

    // Capture initial scalability settings
    if (UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings())
    {
        for (const FName& GroupName : LoweringPriorityOrder)
        {
            int32 CurrentLevel = GetQualityLevelForGroup(GameUserSettings, GroupName);
            InitialScalabilityLevels.Add(GroupName, CurrentLevel);
        }
    }

    // Setup timers
    if (UWorld* World = GetWorld())
    {
        // Sample the frame rate every x seconds (i.e: every 0.5 seconds)
        World->GetTimerManager().SetTimer(FPSSampleTimerHandle, this, &UDynamicQualitySubsystem::SampleFPS, SampleInterval, true);
        // Check if we should actually modify it every x seconds (i.e: every 5 seconds)
        World->GetTimerManager().SetTimer(AdjustmentCheckTimerHandle, this, &UDynamicQualitySubsystem::CheckAndAdjustQuality, 5.0f, true);
    }
}

void UDynamicQualitySubsystem::Deinitialize()
{
    SaveSettings();
    
    Super::Deinitialize();
}

void UDynamicQualitySubsystem::SetTargetFPS(int32 NewTargetFPS)
{
    TargetFPS = NewTargetFPS;
}

void UDynamicQualitySubsystem::SetEnableDynamicQuality(bool bEnable)
{
    bEnableDynamicQuality = bEnable;
}

void UDynamicQualitySubsystem::SampleFPS()
{
    if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
    {
        const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
        if (DeltaTime > 0)
        {
            const float CurrentFPS = 1.0f / DeltaTime;
            FPSBuffer.Add(CurrentFPS);

            if (FPSBuffer.Num() > MaxSamples)
            {
                FPSBuffer.RemoveAt(0);
            }
        }
    }
}

void UDynamicQualitySubsystem::CheckAndAdjustQuality()
{
    if (!bEnableDynamicQuality) return;
    
    const float AverageFPS = GetAverageFPS();
    if (AverageFPS <= 0) return;

    TimeSinceLastAdjustment += 5.0f; // Check every 5 seconds

    if (TimeSinceLastAdjustment >= AdjustmentCooldown)
    {
        if (AverageFPS < TargetFPS)
        {
            AdjustQuality(false); // Lower quality
            TimeSinceLastAdjustment = 0.0f;
        }
        else if (AverageFPS > TargetFPS * 1.1f) // 10% hysteresis
        {
            AdjustQuality(true); // Increase quality
            TimeSinceLastAdjustment = 0.0f;
        }
    }
}

float UDynamicQualitySubsystem::GetAverageFPS() const
{
    if (FPSBuffer.IsEmpty()) return 0.0f;

    float Total = 0.0f;
    for (const float FPS : FPSBuffer)
    {
        Total += FPS;
    }
    return Total / FPSBuffer.Num();
}

void UDynamicQualitySubsystem::AdjustQuality(bool bIncreaseQuality)
{
    UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
    if (!GameUserSettings) return;

    const TArray<FName>& PriorityOrder = bIncreaseQuality ? IncreasingPriorityOrder : LoweringPriorityOrder;

    for (const FName& GroupName : PriorityOrder)
    {
        int32 CurrentLevel = GetQualityLevelForGroup(GameUserSettings, GroupName);
        const int32 MaxLevel = InitialScalabilityLevels[GroupName];
        int32 NewLevel = CurrentLevel;

        if (bIncreaseQuality)
        {
            if (CurrentLevel < MaxLevel) NewLevel++;
        }
        else
        {
            if (CurrentLevel > 0) NewLevel--;
        }

        if (NewLevel != CurrentLevel)
        {
            SetQualityLevelForGroup(GameUserSettings, GroupName, NewLevel);
            GameUserSettings->ApplySettings(false); // Apply changes without saving to disk
            break; // Adjust one group per check
        }
    }
}
void UDynamicQualitySubsystem::SaveSettings()
{
    UVyraSaveGame_DynamicQuality* SaveGameInstance = Cast<UVyraSaveGame_DynamicQuality>(
        UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_DynamicQuality::StaticClass()));

    if (SaveGameInstance)
    {
        // Set values from current settings
        SaveGameInstance->bEnableDynamicQuality = bEnableDynamicQuality;
        SaveGameInstance->TargetFPS = TargetFPS;
		
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("DynamicQualitySlot"), 0))
        {
            UE_LOG(LogTemp, Log, TEXT("DynamicQualitySlot saved successfully!"));
        }
    }
}

void UDynamicQualitySubsystem::LoadSettings()
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("DynamicQualitySlot"), 0))
    {
        UVyraSaveGame_DynamicQuality* SaveGameInstance = Cast<UVyraSaveGame_DynamicQuality>(
            UGameplayStatics::LoadGameFromSlot(TEXT("DynamicQualitySlot"), 0));

        if (SaveGameInstance)
        {
            // Apply loaded settings
            SetEnableDynamicQuality(SaveGameInstance->bEnableDynamicQuality);
            SetTargetFPS(SaveGameInstance->TargetFPS);
            
            UE_LOG(LogTemp, Log, TEXT("DynamicQualitySlot loaded successfully!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No DynamicQualitySlot save found, using defaults."));
    }
}
