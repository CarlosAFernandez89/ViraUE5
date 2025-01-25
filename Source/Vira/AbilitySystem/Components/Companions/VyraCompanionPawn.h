// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VyraCompanionPawn.generated.h"


UCLASS()
class VIRA_API AVyraCompanionPawn : public ACharacter
{
	GENERATED_BODY()

public:
	AVyraCompanionPawn();

	UFUNCTION(BlueprintCallable, Category = "VyraCompanionPawn")
	AActor* GetCompanionOwner() const {return GetOwner(); } ;
};
