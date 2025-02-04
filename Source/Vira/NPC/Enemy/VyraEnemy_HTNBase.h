// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/NPC/VyraNPCWithASC.h"
#include "VyraEnemy_HTNBase.generated.h"

UCLASS()
class VIRA_API AVyraEnemy_HTNBase : public AVyraNPCWithASC
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVyraEnemy_HTNBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HTN")
	TObjectPtr<class UHTN> HTNAsset;

	private:
	UFUNCTION()
	void OnDeathEvent();
};
