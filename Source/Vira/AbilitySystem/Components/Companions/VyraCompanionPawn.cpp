// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraCompanionPawn.h"


// Sets default values
AVyraCompanionPawn::AVyraCompanionPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
