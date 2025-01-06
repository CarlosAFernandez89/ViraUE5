// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VyraSaveGame_AudioSettingsData.generated.h"


/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_AudioSettingsData : public USaveGame
{
	GENERATED_BODY()

public:

	UVyraSaveGame_AudioSettingsData();
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	
	UPROPERTY()
	float MasterVolume = 1.0f;
	
	UPROPERTY()
	float MusicVolume = 1.0f;
	
	UPROPERTY()
	float SoundEffectsVolume = 1.0f;
	
	UPROPERTY()
	float VoiceVolume = 1.0f;

	UPROPERTY()
	float WeatherVolume = 1.0f;
};
