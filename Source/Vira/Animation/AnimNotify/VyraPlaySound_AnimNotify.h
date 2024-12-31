// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimNotify.h"
#include "VyraPlaySound_AnimNotify.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Config = Game, meta=(DisplayName="Play Sound With Variable Pitch"))
class VIRA_API UVyraPlaySound_AnimNotify : public UPaperZDAnimNotify
{
	GENERATED_BODY()

public:
	UVyraPlaySound_AnimNotify(const FObjectInitializer& InObjectInitializer);
	
	// Sound to Play
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	TObjectPtr<USoundBase> Sound;

	// Volume Multiplier
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float VolumeMultiplier;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float PitchMultiplierMin;
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float PitchMultiplierMax;

	// If this sound should follow its owner
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	uint32 bFollow : 1;

#if WITH_EDITORONLY_DATA
	// Should ignore attenuation in the editor and play as a 2D sound?
	UPROPERTY(Config, EditAnywhere, Category = "AnimNotify")
	uint32 bPreviewIgnoreAttenuation:1;
#endif

	// Socket or bone name to attach sound to
	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (EditCondition = "bFollow"))
	FName AttachName;

public:
	void OnReceiveNotify_Implementation(UPaperZDAnimInstance *OwningInstance = nullptr) const override;
	FName GetDisplayName_Implementation() const override;
};
