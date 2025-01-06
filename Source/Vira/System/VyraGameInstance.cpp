// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameInstance.h"
#include "Engine/World.h"
#include "SpudSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/VyraSaveGame_GameInstanceInfo.h"
#include "SaveGame/VyraSaveGame_PlayerData.h"

UVyraGameInstance::UVyraGameInstance(): PlayStartTime(0), TotalSaveSlotsCreated(0)
{
	CurrentSlotName = FString("DevelopmentSlot");
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
	// Display the message on screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,                           // Key (-1 for unique message)
			5.0f,                         // Duration (in seconds)
			FColor::Green,                // Message color
			TEXT("OnLoadedLevelCalled")   // Message text
		);
	}

	if (IsNotMainMenu())
	{
		PlayStartTime = FPlatformTime::Seconds();
	}
	
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
