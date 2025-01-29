// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWidget_PowerUp_Slot.h"

#include "VyraWidget_PowerUp_Panel.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UVyraWidget_PowerUp_Slot::Broadcast_OnSelectedMessage(const FGameplayTag GameplayMessageChannel)
{
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UGameplayMessageSubsystem* GameplayMessageSubsystem = GameInstance->GetSubsystem<UGameplayMessageSubsystem>())
		{
			FPowerUpSlot_Selected PowerUpSlot_Selected;
			PowerUpSlot_Selected.PowerUpDefinition = SelectedPowerUp;
			GameplayMessageSubsystem->BroadcastMessage(GameplayMessageChannel,PowerUpSlot_Selected);
		}
	}
}
