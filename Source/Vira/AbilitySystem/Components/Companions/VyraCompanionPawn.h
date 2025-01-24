// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VyraCompanionPawn.generated.h"

UCLASS()
class VIRA_API AVyraCompanionPawn : public APawn
{
	GENERATED_BODY()

public:
	AVyraCompanionPawn();

	UFUNCTION(BlueprintCallable, Category = "VyraCompanionPawn")
	AActor* GetCompanionOwner() const {return GetOwner(); } ;
};
