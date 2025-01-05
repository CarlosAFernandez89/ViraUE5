// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularPlayerController.h"
#include "VyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API AVyraPlayerController : public AGSCModularPlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UQuestTrackerComponent* QuestTrackerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UQuestComponent* QuestComponent;

public:
	AVyraPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnWindowFocusChanged(bool bIsFocused);


private:
	UPROPERTY()
	class AVyraPlayerStateCharacter* PlayerCharacter;

	UPROPERTY()
	class UCommonInputSubsystem* CommonInputSubsystem;
};
