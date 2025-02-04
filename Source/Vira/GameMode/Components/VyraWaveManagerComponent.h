// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vira/NPC/Enemy/VyraEnemyCharacter.h"
#include "VyraWaveManagerComponent.generated.h"

class AEnemySpawnPoint;
class UEnemyDataAsset;
class AVyraPlayerStateCharacter;
class UNavigationSystemV1;
class UCurveFloat;
class UCurveVector;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UVyraWaveManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UVyraWaveManagerComponent();

    UFUNCTION(BlueprintCallable, Category = "Waves")
    void StartNextWave();

    UFUNCTION(BlueprintCallable, Category = "Waves")
    void EndWave();

    UFUNCTION(BlueprintCallable, Category = "Waves")
    int32 GetCurrentWaveNumber() const { return CurrentWave; }

    UFUNCTION(BlueprintNativeEvent, Category = "Waves")
    void OnEnemyKilled(AVyraEnemyCharacter* Enemy);
    
    // Configuration properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    int32 StartingWave = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    UCurveFloat* WaveExpCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    float WaveSpawnRadiusMax = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    float WaveSpawnRadiusMin = 850.f;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    TSubclassOf<UGameplayEffect> EnemyScaler;

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveStartedSignature, float, WaveDelay, int32, WaveNumber);
    UPROPERTY(BlueprintAssignable, Category = "Waves")
    FOnWaveStartedSignature OnWaveStarted;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEndedSignature);
    UPROPERTY(BlueprintAssignable, Category = "Waves")
    FOnWaveEndedSignature OnWaveEnded;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveExperienceUpdatedSignature, float, CurrentExperience, float, MaxExperience);
    UPROPERTY(BlueprintAssignable, Category = "Waves")
    FOnWaveExperienceUpdatedSignature OnWaveExperienceUpdated;

protected:
    virtual void BeginPlay() override;

private:
    void SpawnEnemiesForWave();
    void SpawnEnemy();
    bool FindGroundPosition(const FVector& StartLocation, FVector& OutGroundLocation) const;
    EVyraEnemyType DetermineEnemyTypeToSpawn() const;
    UEnemyDataAsset* SelectEnemyToSpawn(EVyraEnemyType EnemyType) const;
    UEnemyDataAsset* GetEnemyDataForType(EVyraEnemyType EnemyType) const;
    void ClearRemainingEnemies(bool ActivateDeathAbility = false);
    void TryClearingNullArrayItems();
    float CalculateWaveExpRequirement(int32 WaveNumber) const;
    int32 GetMaxEnemiesToSpawn(int32 WaveNumber) const;
    int32 GetEnemiesToSpawn(int32 WaveNumber) const;
    void ApplyScalingGameplayEffect(const AVyraEnemyCharacter* Owner, EVyraEnemyType InEnemyType) const;

    FTimerHandle WaveTimerHandle;
    int32 CurrentWave = 0;
    float WaveExpRequirement = 0;
    float CurrentWaveExp = 0;
    int32 MaxEnemiesToSpawnThisWave = 0;

    UPROPERTY()
    TArray<AEnemySpawnPoint*> SpawnPoints;

    void CollectSpawnPoints();
    bool GetSpawnTransform(FVector& OutLocation, FRotator& OutRotation);

    UPROPERTY()
    TArray<AVyraEnemyCharacter*> SpawnedEnemies;

    UPROPERTY()
    AVyraPlayerStateCharacter* VyraPlayerCharacter = nullptr;

    UPROPERTY()
    UNavigationSystemV1* NavigationSystem = nullptr;

    const float WaveStartDelay = 2.f;
    const float CurrentWaveSpawnRate = 6.f;
};