// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularGameMode.h"
#include "Vira/NPC/Enemy/VyraEnemyCharacter.h"
#include "VyraGameMode_Arcade.generated.h"

class UPowerUpComponent;
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

	UPROPERTY(visibleAnywhere, BlueprintReadWrite,Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPowerUpComponent> PowerUpComponent;

public:
	AVyraGameMode_Arcade();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPowerUpComponent* GetPowerUpComponent() const { return PowerUpComponent; }
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Waves")
	int32 GetCurrentWaveNumber() const {return CurrentWave; };
	
	UFUNCTION(BlueprintCallable, Category = "Waves")
	void StartNextWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveStarted(float WaveDelay, int32 InCurrentWave);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void EndWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveEnded();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Waves")
	void OnEnemyKilled(AVyraEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void SpawnEnemiesForWave();

	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void OnWaveExperienceUpdated(float CurrentExperience, float MaxExperience);

private:

	UEnemyDataAsset* SelectEnemyToSpawn(EVyraEnemyType EnemyType);

	EVyraEnemyType DetermineEnemyTypeToSpawn();

	void SpawnEnemy();

	void ClearRemainingEnemies(bool ActivateDeathAbility = false);
	
	UEnemyDataAsset* GetEnemyDataForType(EVyraEnemyType EnemyType);
	
	float CalculateWaveExpRequirement(int32 WaveNumber);
	
	int32 GetMaxEnemiesToSpawn(int32 WaveNumber);
	
	int32 GetEnemiesToSpawn(int32 WaveNumber);

	void ApplyScalingGameplayEffect(const AVyraEnemyCharacter* Owner, EVyraEnemyType InEnemyType);
	
	void TryClearingNullArrayItems();

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	int32 StartingWave = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TSubclassOf<UGameplayEffect> EnemyScaler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UCurveFloat* WaveExpCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	float WaveSpawnRadiusMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	float WaveSpawnRadiusMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UCurveFloat* MaxEnemiesPerWaveCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	UCurveVector* RangeOfEnemiesPerWaveCurve;
	
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
	float WaveStartDelay = 2.f;

	UPROPERTY()
	int32 EnemiesToSpawnThisWave = 4;

	UPROPERTY()
	int32 MaxEnemiesToSpawnThisWave;
	
	UPROPERTY()
	TArray<AVyraEnemyCharacter*> SpawnedEnemies;

	UPROPERTY()
	FTimerHandle WaveTimerHandle;
};
