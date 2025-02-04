// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemy_HTNBase.h"

#include "Components/GSCCoreComponent.h"
#include "HTN/Public/HTNBlueprintLibrary.h"

// Sets default values
AVyraEnemy_HTNBase::AVyraEnemy_HTNBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVyraEnemy_HTNBase::BeginPlay()
{
	Super::BeginPlay();

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (HTNAsset)
		{
			UHTNBlueprintLibrary::RunHTN(AIController, HTNAsset);
		}
	}

	GetGSCCoreComponent()->OnDeath.AddDynamic(this, &AVyraEnemy_HTNBase::OnDeathEvent);
}

void AVyraEnemy_HTNBase::OnDeathEvent()
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(FString("Death"));
		}
	}
}
