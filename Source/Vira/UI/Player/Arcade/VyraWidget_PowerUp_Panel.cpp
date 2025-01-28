// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWidget_PowerUp_Panel.h"




void UVyraWidget_PowerUp_Panel::AddPowerUpToArray(const FPowerUpData InPowerUp)
{
	if (!InPowerUp.PowerUpName.IsNone())
	{
		PowerUpsData.Add(InPowerUp);
	}
}
