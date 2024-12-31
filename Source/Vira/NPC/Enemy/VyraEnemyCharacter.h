// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/NPC/VyraNPCCharacterBase.h"
#include "VyraEnemyCharacter.generated.h"

UCLASS()
class VIRA_API AVyraEnemyCharacter : public AVyraNPCCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
