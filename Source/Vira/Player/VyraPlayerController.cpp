// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraPlayerController.h"


void AVyraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FSlateApplication::Get().OnApplicationActivationStateChanged()
	.AddUObject(this, &AVyraPlayerController::OnWindowFocusChanged);
}

void AVyraPlayerController::OnWindowFocusChanged_Implementation(bool bIsFocused)
{
	// Don't pause in the editor, it's annoying
#if !WITH_EDITOR
	if (bIsFocused)
	{
		// Unlimit game FPS
		GEngine->SetMaxFPS(0);

		// Unpause the game
		// MyHUD->SetPause(false);
	}
	else
	{
		// Reduce FPS to max 10 while in the background
		GEngine->SetMaxFPS(10.0f);

		// Pause the game, using your "show pause menu" function
		// MyHUD->SetPause(true);
	}
#endif
}
