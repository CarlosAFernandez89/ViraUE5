// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatableWidget_AudioOptionsPanel.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

void UActivatableWidget_AudioOptionsPanel::SetAndTrackSoundClassVolume(UWorld* WorldContextObject, USoundMix* SoundMix,
	USoundClass* SoundClass, float Volume)
{
	if (!WorldContextObject || !SoundMix || !SoundClass) return;

	// Set the sound mix class override
	UGameplayStatics::SetSoundMixClassOverride(WorldContextObject, SoundMix, SoundClass, Volume, 1.0f, true);

	// Track the volume in a map or variable
	TrackedSoundClassVolumes.Add(SoundClass, Volume);
}

float UActivatableWidget_AudioOptionsPanel::GetTrackedSoundClassVolume(USoundClass* SoundClass) const
{
	if (TrackedSoundClassVolumes.Contains(SoundClass))
	{
		return TrackedSoundClassVolumes[SoundClass];
	}

	return 1.0f; // Default volume
}

float UActivatableWidget_AudioOptionsPanel::GetSoundClassVolume(USoundClass* SoundClass)
{
	if (SoundClass)
	{
		return SoundClass->Properties.Volume;
	}
	return -1.f;
}
