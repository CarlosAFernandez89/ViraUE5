// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadePassive_SoulSiphon.h"

UArcadePassive_SoulSiphon::UArcadePassive_SoulSiphon()
{
	bActivateOnGranted = true;

	AbilityInfo.Name = "SoulSiphon";
	AbilityInfo.Description = "Increases health recovery on each kill or through damage absorption on special abilities.";
}
