// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingsSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Vira/System/SaveGame/VyraSaveGame_GameSettings.h"

void UGameSettingsSubsystem::SetShowEnemyHealthBars(bool bValue)
{
	bShowEnemyHealthBars = bValue;
	OnShowHealthBarsUpdated.Broadcast(bShowEnemyHealthBars);
}

void UGameSettingsSubsystem::SetShowBlood(bool bValue)
{
	bShowBlood = bValue;
}

void UGameSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadSettings();
}

void UGameSettingsSubsystem::Deinitialize()
{
	SaveSettings();
	
	Super::Deinitialize();
}

void UGameSettingsSubsystem::SaveSettings()
{
	UVyraSaveGame_GameSettings* SaveGameInstance = Cast<UVyraSaveGame_GameSettings>(
		UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_GameSettings::StaticClass()));

	if (SaveGameInstance)
	{
		// Set values from current settings
		SaveGameInstance->bShowDamageNumbers = bShowDamageNumbers;
		SaveGameInstance->bEnableCameraShake = bEnableCameraShake;
		SaveGameInstance->bShowEnemyHealthBars = bShowEnemyHealthBars;
		SaveGameInstance->bShowBlood = bShowBlood;
		
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GameSettingsSlot"), 0))
		{
			UE_LOG(LogTemp, Log, TEXT("GameSettings saved successfully!"));
		}
	}
}

void UGameSettingsSubsystem::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("GameSettingsSlot"), 0))
	{
		UVyraSaveGame_GameSettings* SaveGameInstance = Cast<UVyraSaveGame_GameSettings>(
			UGameplayStatics::LoadGameFromSlot(TEXT("GameSettingsSlot"), 0));

		if (SaveGameInstance)
		{
			// Apply loaded settings
			bShowDamageNumbers = SaveGameInstance->bShowDamageNumbers;
			bEnableCameraShake = SaveGameInstance->bEnableCameraShake;
			bShowEnemyHealthBars = SaveGameInstance->bShowEnemyHealthBars;
			bShowBlood = SaveGameInstance->bShowBlood;

			UE_LOG(LogTemp, Log, TEXT("GameSettings loaded successfully!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No GameSettings save found, using defaults."));
	}
}
