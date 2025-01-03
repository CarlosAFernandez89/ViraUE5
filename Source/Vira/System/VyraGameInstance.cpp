// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameInstance.h"
#include "Engine/World.h"
#include "SpudSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/VyraSaveGame_PlayerData.h"

UVyraGameInstance::UVyraGameInstance()
{
	CurrentSlotName = FString("DevelopmentSlot");
}

void UVyraGameInstance::Init()
{
	Super::Init();

	// Bind to the level loaded delegate
	FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UVyraGameInstance::OnLevelLoaded);

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

	LoadCurrentPlayerData();
	LoadCurrentLevel();
}

FString UVyraGameInstance::GetCurrentSaveSlotName()
{
	return CurrentSlotName;
}
