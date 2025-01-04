// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAIController.h"


// Sets default values
AVyraAIController::AVyraAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVyraAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVyraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

