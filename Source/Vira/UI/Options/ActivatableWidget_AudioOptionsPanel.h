// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/UI/Foundation/VyraActivatableWidget.h"
#include "ActivatableWidget_AudioOptionsPanel.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UActivatableWidget_AudioOptionsPanel : public UVyraActivatableWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Audio", meta = (WorldContext = "WorldContextObject"))
	void SetAndTrackSoundClassVolume(UWorld* WorldContextObject, USoundMix* SoundMix, USoundClass* SoundClass, float Volume);

	UFUNCTION(BlueprintCallable, Category = "Audio", meta = (WorldContext = "WorldContextObject"))
	float GetTrackedSoundClassVolume(USoundClass* SoundClass) const;


	UFUNCTION(BlueprintCallable, Category = "Audio")
	static float GetSoundClassVolume(USoundClass* SoundClass);

	private:
	TMap<USoundClass*, float> TrackedSoundClassVolumes;
};
