﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingsSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Vira/System/SaveGame/VyraSaveGame_GameSettings.h"

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

			UE_LOG(LogTemp, Log, TEXT("GameSettings loaded successfully!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No GameSettings save found, using defaults."));
	}
}
