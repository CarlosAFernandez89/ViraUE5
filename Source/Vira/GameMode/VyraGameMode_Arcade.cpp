// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameMode_Arcade.h"

#include "Components/PowerUpComponent.h"
#include "Components/VyraWaveManagerComponent.h"

AVyraGameMode_Arcade::AVyraGameMode_Arcade()
{
	PowerUpComponent = CreateDefaultSubobject<UPowerUpComponent>("PowerUpComponent");
	WaveManagerComponent = CreateDefaultSubobject<UVyraWaveManagerComponent>("WaveManagerComponent");
}