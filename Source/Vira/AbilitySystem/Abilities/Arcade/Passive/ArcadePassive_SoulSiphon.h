// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "Vira/NPC/Enemy/VyraEnemyCharacter.h"
#include "ArcadePassive_SoulSiphon.generated.h"

USTRUCT(BlueprintType)
struct FSoulSiphonGameplayMessage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVyraEnemyType EnemyType = EVyraEnemyType::NORMAL;
};
/**
 * 
 */
UCLASS()
class VIRA_API UArcadePassive_SoulSiphon : public UVyraGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UArcadePassive_SoulSiphon();

	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoulSiphon|GameplayEffect")
	TSubclassOf<UGameplayEffect> SoulSiphonEffectClass;
	
};
