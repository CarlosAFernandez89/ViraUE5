﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameInstance.h"

#include "AbilitySystemGlobals.h"
#include "Engine/World.h"
#include "SpudSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/VyraSaveGame_AudioSettingsData.h"
#include "SaveGame/VyraSaveGame_GameInstanceInfo.h"
#include "SaveGame/VyraSaveGame_PlayerData.h"
#include "Sound/SoundClass.h"


UVyraGameInstance::UVyraGameInstance(): PlayStartTime(0), TotalSaveSlotsCreated(0), CurrentSoundMix(nullptr),
                                        CurrentMasterClass(nullptr),
                                        CurrentMusicClass(nullptr),
                                        CurrentSoundEffectsClass(nullptr),
                                        CurrentVoiceClass(nullptr), CurrentWeatherClass(nullptr)
{
	CurrentSlotName = FString("DevelopmentSlot");

	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UVyraGameInstance::Init()
{
	Super::Init();
	
	// Bind to the level loaded delegate
	FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UVyraGameInstance::OnLevelLoaded);

	if (IsNotMainMenu())
	{
		PlayStartTime = FPlatformTime::Seconds();
	}
}

void UVyraGameInstance::LoadGameInstanceInfo()
{
	//Load save slots info
	if (UGameplayStatics::DoesSaveGameExist("GameInstanceInfo", 0))
	{
		if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot("GameInstanceInfo", 0))
		{
			if (UVyraSaveGame_GameInstanceInfo* SGI = Cast<UVyraSaveGame_GameInstanceInfo>(SaveGame))
			{
				AllSaveSlots = SGI->AllSaveSlots;
				LastSaveSlotLoaded = SGI->LastSaveSlotLoaded;
				TotalSaveSlotsCreated = SGI->TotalSaveSlotsCreated;
				CurrentSlotName = LastSaveSlotLoaded;
				UpdateSaveSlotName(LastSaveSlotLoaded);
			}
		}
	}
	else
	{
		if (UVyraSaveGame_GameInstanceInfo* SGI = Cast<UVyraSaveGame_GameInstanceInfo>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_GameInstanceInfo::StaticClass())))
		{
			// Creating the first savegameinstance
			UGameplayStatics::SaveGameToSlot(SGI,SGI->SaveSlotName, SGI->UserIndex);
		}
	}
}

void UVyraGameInstance::SaveAudioSettings()
{
	if (UVyraSaveGame_AudioSettingsData* AudioSG = Cast<UVyraSaveGame_AudioSettingsData>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_AudioSettingsData::StaticClass())))
	{
		AudioSG->MasterVolume = SoundMixOverrides.FindRef(CurrentMasterClass);
		AudioSG->MusicVolume = SoundMixOverrides.FindRef(CurrentMusicClass);
		AudioSG->SoundEffectsVolume = SoundMixOverrides.FindRef(CurrentSoundEffectsClass);
		AudioSG->VoiceVolume = SoundMixOverrides.FindRef(CurrentVoiceClass);
		AudioSG->WeatherVolume = SoundMixOverrides.FindRef(CurrentWeatherClass);
		
		if (!UGameplayStatics::SaveGameToSlot(AudioSG,AudioSG->SaveSlotName, AudioSG->UserIndex))
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, FString::Printf(TEXT("Failed to save Audio Settings")));
		}
	}
}

void UVyraGameInstance::LoadAudioSettings()
{
	if (UGameplayStatics::DoesSaveGameExist("AudioSettings", 0))
	{
		if (USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot("AudioSettings", 0))
		{
			if (UVyraSaveGame_AudioSettingsData* AudioSG = Cast<UVyraSaveGame_AudioSettingsData>(SaveGame))
			{
				ApplySoundMixClassOverride(CurrentSoundMix, CurrentMasterClass, AudioSG->MasterVolume, 1,1);
				ApplySoundMixClassOverride(CurrentSoundMix, CurrentMusicClass, AudioSG->MusicVolume, 1,1);
				ApplySoundMixClassOverride(CurrentSoundMix, CurrentSoundEffectsClass, AudioSG->SoundEffectsVolume, 1,1);
				ApplySoundMixClassOverride(CurrentSoundMix, CurrentVoiceClass, AudioSG->VoiceVolume, 1,1);
				ApplySoundMixClassOverride(CurrentSoundMix, CurrentWeatherClass, AudioSG->WeatherVolume, 1,1);

			}
		}
	}
	else
	{
		if (UVyraSaveGame_AudioSettingsData* AudioSG = Cast<UVyraSaveGame_AudioSettingsData>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_AudioSettingsData::StaticClass())))
		{
			// Creating the first savegameinstance
			UGameplayStatics::SaveGameToSlot(AudioSG,AudioSG->SaveSlotName, AudioSG->UserIndex);
		}
	}
}

float UVyraGameInstance::GetAudioVolumeFromClass(USoundClass* ClassToFind) const
{
	return SoundMixOverrides.FindRef(ClassToFind);
}


void UVyraGameInstance::CreateNewSaveSlot()
{
	FString NewName = FString("Slot_") + FString::FromInt(++TotalSaveSlotsCreated);
	
	if (UVyraSaveGame_GameInstanceInfo* SGI = Cast<UVyraSaveGame_GameInstanceInfo>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_GameInstanceInfo::StaticClass())))
	{
		AllSaveSlots.Add(NewName);
		LastSaveSlotLoaded = NewName;
		UpdateSaveSlotName(NewName);
		CurrentSlotName = NewName;
		
		UpdateGameInstanceSaveFile();
		
	}
}

void UVyraGameInstance::UpdateGameInstanceSaveFile()
{
	if (UVyraSaveGame_GameInstanceInfo* SGI = Cast<UVyraSaveGame_GameInstanceInfo>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_GameInstanceInfo::StaticClass())))
	{
		SGI->AllSaveSlots = AllSaveSlots;
		SGI->LastSaveSlotLoaded = LastSaveSlotLoaded;
		SGI->TotalSaveSlotsCreated = TotalSaveSlotsCreated;
		UGameplayStatics::SaveGameToSlot(SGI,SGI->SaveSlotName, SGI->UserIndex);
	}
}


bool UVyraGameInstance::LoadSaveSlot(const FString& SlotName)
{
	if (int index = AllSaveSlots.Find(SlotName); index != INDEX_NONE)
	{
		UpdateSaveSlotName(AllSaveSlots[index]);
		LastSaveSlotLoaded = AllSaveSlots[index];
		CurrentSlotName = AllSaveSlots[index];
		UpdateGameInstanceSaveFile();
		return true;
	}
	return false;
}

bool UVyraGameInstance::DeleteSaveSlot(const FString& SlotName)
{
	if (int index = AllSaveSlots.Find(SlotName); index != INDEX_NONE)
	{
		if (LastSaveSlotLoaded == SlotName)
		{
			LastSaveSlotLoaded = FString("");
		}
		AllSaveSlots.RemoveAt(index);
		UpdateGameInstanceSaveFile();
		return true;
	}
	return false;
}

void UVyraGameInstance::UpdateSaveSlotName(const FString& NewSaveSlotName)
{
	CurrentLevelSaveSlot = NewSaveSlotName;
}

void UVyraGameInstance::SaveCurrentPlayerData()
{
	if (UVyraSaveGame_PlayerData* PlayerDataSave = Cast<UVyraSaveGame_PlayerData>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_PlayerData::StaticClass())))
	{
		FString SaveSlotName = "VyraSaveGame_PlayerData_" + CurrentSlotName; // Or more complex naming
		PlayerDataSave->SaveAllPlayerData(SaveSlotName, this);
	}
}

void UVyraGameInstance::LoadCurrentPlayerData()
{
	FString SaveSlotName = "VyraSaveGame_PlayerData_" + CurrentSlotName; // Or more complex naming

	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0 ))
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindUObject(this, &UVyraGameInstance::AsyncLoadPlayerDataComplete);
		UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, LoadedDelegate);
	}
}

void UVyraGameInstance::AsyncLoadPlayerDataComplete(const FString& SlotName, int UserIndex,
	USaveGame* LoadedGameData)
{
	if (UVyraSaveGame_PlayerData* PlayerData = Cast<UVyraSaveGame_PlayerData>(LoadedGameData))
	{
		PlayerData->LoadAllPlayerData(this);
	}
}

void UVyraGameInstance::SaveCurrentLevel()
{
	// 1. Get the current level's name:
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

	// 2. Create a unique save slot name (consider adding player name, etc.):
	FString SaveSlotName = CurrentLevelName + "_" + CurrentSlotName; // Or more complex naming
	CurrentLevelSaveSlot = SaveSlotName;

	if (USpudSubsystem* SpudSystem = GetSpudSubsystem(GetWorld()))
	{
		SpudSystem->SaveGame(CurrentLevelSaveSlot, FText::FromString(CurrentLevelSaveSlot));
	}
}

void UVyraGameInstance::LoadCurrentLevel()
{
	// 1. Get the current level's name:
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);

	// 2. Create a unique save slot name (consider adding player name, etc.):
	FString SaveSlotName = CurrentLevelName + "_" + CurrentSlotName; // Or more complex naming
	CurrentLevelSaveSlot = SaveSlotName;
	
	if (USpudSubsystem* SpudSystem = GetSpudSubsystem(GetWorld()))
	{
		SpudSystem->LoadGame(CurrentLevelSaveSlot);
	}
	
}

void UVyraGameInstance::OnLevelLoaded(const FActorsInitializedParams& Params)
{
	if (IsNotMainMenu())
	{
		PlayStartTime = FPlatformTime::Seconds();
	}

	LoadAudioSettings();

	//No need loading in arcade mode. Everything gets reset.
	if (!IsNotArcadeMode()) return;
	
	LoadGameInstanceInfo();
	LoadCurrentPlayerData();
	LoadCurrentLevel();
}

FString UVyraGameInstance::GetCurrentSaveSlotName()
{
	return CurrentSlotName;
}

void UVyraGameInstance::GetSaveSlotPlayerData(const FString& SlotName ,float& CurrentSouls, float& Progress,
	FString& PlayTimeString, FString& Location)
{
	const FString SaveSlotName = FString("VyraSaveGame_PlayerData_" + SlotName);
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0 ))
	{
		if (UVyraSaveGame_PlayerData* SGPD = Cast<UVyraSaveGame_PlayerData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
		{
			FSaveGamePlayerData_Currency Currency = SGPD->GetLoadedCurrency();
			CurrentSouls = Currency.Souls;
			Progress = 1.f;
			float PlayTime = SGPD->GetTotalPlayTime();
			PlayTimeString = FormatPlayTime(PlayTime);
			Location = FString("Place Location");
		}
	}
}

float UVyraGameInstance::GetPlayTime()
{
	if (IsNotMainMenu())
	{
		// Calculate the session time played
		float SessionPlayTime = FPlatformTime::Seconds() - PlayStartTime;
		PlayStartTime = FPlatformTime::Seconds();

		const FString SaveSlotName = FString("VyraSaveGame_PlayerData_" + CurrentSlotName);
		if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,0 ))
		{
			if (UVyraSaveGame_PlayerData* SGPD = Cast<UVyraSaveGame_PlayerData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
			{
				SessionPlayTime += SGPD->GetTotalPlayTime();
			}
		}
		return SessionPlayTime;
	}
	
	return 0.f;
}

FString UVyraGameInstance::FormatPlayTime(float TotalSeconds)
{
	int32 Hours = FMath::FloorToInt(TotalSeconds / 3600);
	int32 Minutes = FMath::FloorToInt((TotalSeconds / 60)) % 60;
	int32 Seconds = FMath::FloorToInt(TotalSeconds) % 60;

	return FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
}

FString UVyraGameInstance::GetCurrentSessionTimeAsString()
{
	return FormatPlayTime(PlayStartTime);
}


void UVyraGameInstance::ApplySoundMixClassOverride(USoundMix* SoundMix, USoundClass* SoundClass,
	float Volume,float Pitch, float FadeInTime)
{
	if (!SoundMix || !SoundClass)
	{
		GEngine->AddOnScreenDebugMessage(-1,8.0f, FColor::Black, FString::Printf(TEXT("nullptr SoundMix or SoundClass")));
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume, Pitch, FadeInTime);
	SoundMixOverrides.Add(SoundClass, Volume);
}

bool UVyraGameInstance::IsNotMainMenu() const
{
	if (UWorld* World = GetWorld())
	{
		FString CurrentLevelName = World->GetMapName();

		// Remove PIE prefix if in editor
		if (CurrentLevelName.StartsWith(TEXT("UEDPIE_0_")))
		{
			CurrentLevelName = CurrentLevelName.RightChop(9); // RightChop removes the first 9 characters.
		}

		if (CurrentLevelName != TEXT("LV_MainMenu")) // Replace "LV_MainMenu" with the actual name of your main menu level
		{
			return true;
		}
	}
	return false;
}

bool UVyraGameInstance::IsNotArcadeMode() const
{
	if (UWorld* World = GetWorld())
	{
		FString CurrentLevelName = World->GetMapName();

		// Remove PIE prefix if in editor
		if (CurrentLevelName.StartsWith(TEXT("UEDPIE_0_")))
		{
			CurrentLevelName = CurrentLevelName.RightChop(9); // RightChop removes the first 9 characters.
		}

		if (CurrentLevelName != TEXT("LV_ArcadeMode")) // Replace "LV_MainMenu" with the actual name of your main menu level
		{
			return true;
		}
	}
	return false;
}