// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemyCharacter.h"


// Sets default values
AVyraEnemyCharacter::AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EnemyType(EVyraEnemyType::NORMAL)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AVyraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
