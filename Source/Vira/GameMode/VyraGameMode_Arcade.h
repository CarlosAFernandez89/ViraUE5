// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularGameMode.h"
#include "Vira/NPC/Enemy/VyraEnemyCharacter.h"
#include "VyraGameMode_Arcade.generated.h"

class UNavigationSystemV1;
class AVyraPlayerStateCharacter;
class UEnemyDataAsset;
class AVyraEnemyCharacter;
/**
 * 
 */
UCLASS()
class VIRA_API AVyraGameMode_Arcade : public AGSCModularGameModeBase
{
	GENERATED_BODY()

public:
		AVyraGameMode_Arcade();

	virtual void BeginPlay() override;
	
protected:
	
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartNextWave();

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void EndWave();

	UFUNCTION(BlueprintNativeEvent, Category = "Waves")
	void OnEnemyKilled(TSubclassOf<AVyraEnemyCharacter> Enemy);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void SpawnEnemiesForWave();

private:

	UEnemyDataAsset* SelectEnemyToSpawn(EVyraEnemyType EnemyType);

	EVyraEnemyType DetermineEnemyTypeToSpawn();

	void SpawnEnemy();

	void ClearRemainingEnemies(bool ActivateDeathAbility = false);
	
	UEnemyDataAsset* GetEnemyDataForClass(TSubclassOf<AVyraEnemyCharacter> EnemyClass);
	
	float CalculateWaveExpRequirement(int32 WaveNumber);

public:

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UCurveFloat* WaveExpCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	float WaveSpawnRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UEnemyDataAsset* BasicEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UEnemyDataAsset* EliteEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UEnemyDataAsset* BossEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UEnemyDataAsset* ArchbossEnemies;

	UPROPERTY()
	AVyraPlayerStateCharacter* VyraPlayerStateCharacter;

	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;
	
private:
	UPROPERTY()
	int32 CurrentWave = 0;

	UPROPERTY()
	float WaveExpRequirement = 0;

	UPROPERTY()
	float CurrentWaveExp = 0;

	UPROPERTY()
	float CurrentWaveSpawnRate = 6.f;

	UPROPERTY()
	int32 EnemiesToSpawnThisWave = 4;
	
	UPROPERTY()
	TArray<AVyraEnemyCharacter*> SpawnedEnemies;

	UPROPERTY()
	FTimerHandle WaveTimerHandle;
};
