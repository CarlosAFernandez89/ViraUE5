#pragma once

#include "CoreMinimal.h"
#include "VyraEnemyCharacter.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"


UCLASS(Blueprintable)
class VIRA_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TArray<TSubclassOf<AVyraEnemyCharacter>> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float ExperienceValue;
	
};