// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularAIController.h"
#include "VyraAIController.generated.h"

UCLASS()
class VIRA_API AVyraAIController : public AGSCModularAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVyraAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
