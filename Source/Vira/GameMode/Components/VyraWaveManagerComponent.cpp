// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWaveManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Curves/CurveVector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Vira/AbilitySystem/Abilities/Enemies/VyraGameplayAbility_EnemyDeath.h"
#include "Vira/Gameplay/Arcade/ArcadeAltar.h"
#include "Vira/Interaction/IInteractableTarget.h"
#include "Vira/NPC/Enemy/EnemyDataAsset.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"


UVyraWaveManagerComponent::UVyraWaveManagerComponent(): WaveExpCurve(nullptr), MaxEnemiesPerWaveCurve(nullptr),
                                                        RangeOfEnemiesPerWaveCurve(nullptr), BasicEnemies(nullptr),
                                                        EliteEnemies(nullptr),
                                                        BossEnemies(nullptr),
                                                        ArchbossEnemies(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UVyraWaveManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWave = StartingWave;
    VyraPlayerCharacter = UVyraBlueprintFunctionLibrary::GetVyraPlayerCharacter(GetWorld());
    NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    CurrentWaveExp = 0.0f;
    OnWaveExperienceUpdated.Broadcast(CurrentWaveExp, WaveExpRequirement);
}

void UVyraWaveManagerComponent::StartNextWave()
{    
    CurrentWave++;
    WaveExpRequirement = CalculateWaveExpRequirement(CurrentWave);
    MaxEnemiesToSpawnThisWave = GetMaxEnemiesToSpawn(CurrentWave);
    CurrentWaveExp = 0.0f;
    
    OnWaveExperienceUpdated.Broadcast(CurrentWaveExp, WaveExpRequirement);
    SpawnEnemiesForWave();
}

void UVyraWaveManagerComponent::EndWave()
{
    GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
    ClearRemainingEnemies();
    OnWaveEnded.Broadcast();
}

void UVyraWaveManagerComponent::SpawnEnemiesForWave()
{
    OnWaveStarted.Broadcast(WaveStartDelay, CurrentWave);
    GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UVyraWaveManagerComponent::SpawnEnemy, 
        CurrentWaveSpawnRate, true, WaveStartDelay);
}

void UVyraWaveManagerComponent::SpawnEnemy()
{
     if (!VyraPlayerCharacter || !NavigationSystem) return;

    TryClearingNullArrayItems();

    const int32 RemainingEnemies = MaxEnemiesToSpawnThisWave - SpawnedEnemies.Num();
    if (RemainingEnemies <= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
        return;
    }

    const int32 EnemiesToSpawnNow = FMath::Min(GetEnemiesToSpawn(CurrentWave), RemainingEnemies);
    const FVector SpawnCenter = VyraPlayerCharacter->GetActorLocation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Cache navigation query filter
    ANavigationData* NavData = NavigationSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
    const FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, nullptr);

    for (int32 i = 0; i < EnemiesToSpawnNow; ++i)
    {
        const EVyraEnemyType EnemyType = DetermineEnemyTypeToSpawn();
        if (UEnemyDataAsset* EnemyData = SelectEnemyToSpawn(EnemyType))
        {
            TSubclassOf<AVyraEnemyCharacter> EnemyClass = EnemyData->EnemyClass[FMath::RandRange(0, EnemyData->EnemyClass.Num() - 1)];
            FNavLocation SpawnLocation;

            // Improved navigation query with proper height checks
            const bool bFoundLocation = NavigationSystem->GetRandomReachablePointInRadius(
                SpawnCenter,
                WaveSpawnRadiusMax,
                SpawnLocation,
                NavData,
                NavFilter
            );

            if (bFoundLocation)
            {
                // Ensure minimum spawn distance
                const float Distance = FVector::Dist2D(SpawnLocation.Location, SpawnCenter);
                if (Distance < WaveSpawnRadiusMin)
                {
                    const FVector Direction = (SpawnLocation.Location - SpawnCenter).GetSafeNormal2D();
                    SpawnLocation.Location = SpawnCenter + Direction * WaveSpawnRadiusMin;
                    
                    // Project to navigation mesh again after adjustment
                    NavigationSystem->ProjectPointToNavigation(
                        SpawnLocation.Location,
                        SpawnLocation,
                        FVector(500, 500, 500), // Extent
                        NavData,
                        NavFilter
                    );
                }

                // Find actual ground position
                FVector GroundLocation;
                if (FindGroundPosition(SpawnLocation.Location, GroundLocation))
                {
                    const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(GroundLocation, SpawnCenter);
                    
                    if (AVyraEnemyCharacter* Enemy = GetWorld()->SpawnActor<AVyraEnemyCharacter>(
                        EnemyClass,
                        GroundLocation,
                        SpawnRotation,
                        SpawnParams))
                    {
                        // Force physics update
                        Enemy->SetActorEnableCollision(true);
                        Enemy->GetMesh()->SetSimulatePhysics(false);
                        Enemy->GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
                        
                        Enemy->OnEnemyKilled.AddDynamic(this, &UVyraWaveManagerComponent::OnEnemyKilled);
                        SpawnedEnemies.Add(Enemy);
                        ApplyScalingGameplayEffect(Enemy, Enemy->GetEnemyType());
                    }
                }
            }
        }
    }
}

bool UVyraWaveManagerComponent::FindGroundPosition(const FVector& StartLocation, FVector& OutGroundLocation) const
{
    const FVector TraceStart = StartLocation + FVector(0, 0, 500);  // Start above
    const FVector TraceEnd = StartLocation - FVector(0, 0, 1000);  // End below

    FHitResult HitResult;
    const TArray<AActor*> IgnoreActors;
    const bool bHit = UKismetSystemLibrary::LineTraceSingle(
        GetWorld(),
        TraceStart,
        TraceEnd,
        UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
        false,
        IgnoreActors,
        EDrawDebugTrace::None,
        HitResult,
        true
    );

    if (bHit && HitResult.bBlockingHit)
    {
        OutGroundLocation = HitResult.Location + FVector(0, 0, 50);  // Offset from ground
        return true;
    }
    
    return false;
}

EVyraEnemyType UVyraWaveManagerComponent::DetermineEnemyTypeToSpawn() const
{
    const float RandomValue = FMath::FRand();
    
    if (CurrentWave < 5)
    {
        return RandomValue < 0.8f ? EVyraEnemyType::NORMAL : EVyraEnemyType::ELITE;
    }
    if (CurrentWave < 10)
    {
        if (RandomValue < 0.6f) return EVyraEnemyType::NORMAL;
        if (RandomValue < 0.9f) return EVyraEnemyType::ELITE;
        return EVyraEnemyType::BOSS;
    }
    
    if (RandomValue < 0.4f) return EVyraEnemyType::NORMAL;
    if (RandomValue < 0.7f) return EVyraEnemyType::ELITE;
    if (RandomValue < 0.9f) return EVyraEnemyType::BOSS;
    return EVyraEnemyType::ARCHBOSS;
}

UEnemyDataAsset* UVyraWaveManagerComponent::SelectEnemyToSpawn(EVyraEnemyType EnemyType) const
{
    if (UEnemyDataAsset* EnemyData = GetEnemyDataForType(EnemyType))
    {
        if (EnemyData->EnemyClass.Num() > 0)
        {
            return EnemyData;
        }
        UE_LOG(LogTemp, Warning, TEXT("Empty EnemyClass array in data asset for type %s!"), 
               *UEnum::GetValueAsString(EnemyType));
    }
    return nullptr;
}

UEnemyDataAsset* UVyraWaveManagerComponent::GetEnemyDataForType(EVyraEnemyType EnemyType) const
{
    switch (EnemyType)
    {
    case EVyraEnemyType::NORMAL: return BasicEnemies;
    case EVyraEnemyType::ELITE: return EliteEnemies;
    case EVyraEnemyType::BOSS: return BossEnemies;
    case EVyraEnemyType::ARCHBOSS: return ArchbossEnemies;
    default: return nullptr;
    }
}

void UVyraWaveManagerComponent::ClearRemainingEnemies(bool ActivateDeathAbility)
{
    for (AVyraEnemyCharacter* Enemy : SpawnedEnemies)
    {
        if (!Enemy) continue;

        if (ActivateDeathAbility)
        {
            if (UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent())
            {
                ASC->TryActivateAbilityByClass(UVyraGameplayAbility_EnemyDeath::StaticClass());
            }
        }
        else
        {
            Enemy->Destroy();
        }
    }
    SpawnedEnemies.Empty();
}

void UVyraWaveManagerComponent::TryClearingNullArrayItems()
{
    SpawnedEnemies.RemoveAllSwap([](const AVyraEnemyCharacter* Enemy) {
        return Enemy == nullptr || !Enemy->IsValidLowLevel();
    });
}

float UVyraWaveManagerComponent::CalculateWaveExpRequirement(int32 WaveNumber) const
{
    return WaveExpCurve ? WaveExpCurve->GetFloatValue(WaveNumber) : 100.0f;
}

int32 UVyraWaveManagerComponent::GetMaxEnemiesToSpawn(int32 WaveNumber) const
{
    return MaxEnemiesPerWaveCurve ? 
        FMath::CeilToInt32(MaxEnemiesPerWaveCurve->GetFloatValue(WaveNumber)) : 25;
}

int32 UVyraWaveManagerComponent::GetEnemiesToSpawn(int32 WaveNumber) const
{
    if (!RangeOfEnemiesPerWaveCurve) return FMath::RandRange(4, 10);

    const FVector RangeVector = RangeOfEnemiesPerWaveCurve->GetVectorValue(WaveNumber);
    int32 Min = FMath::RoundToInt(RangeVector.X);
    int32 Max = FMath::RoundToInt(RangeVector.Y);
    
    if (Min > Max) Swap(Min, Max);
    return FMath::RandRange(Min, Max);
}

void UVyraWaveManagerComponent::ApplyScalingGameplayEffect(const AVyraEnemyCharacter* Owner, EVyraEnemyType InEnemyType) const
{
    if (!Owner) return;

    UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
    if (!ASC) return;

    if (UEnemyDataAsset* Data = GetEnemyDataForType(InEnemyType))
    {
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EnemyScaler, 1, Context);
        
        if (SpecHandle.IsValid())
        {
            static const FGameplayTag MaxHealthTag = FGameplayTag::RequestGameplayTag("SetByCaller.Arcade.MaxHealth");
            static const FGameplayTag BaseDamageTag = FGameplayTag::RequestGameplayTag("SetByCaller.Arcade.BaseDamage");
            static const FGameplayTag DamageReductionTag = FGameplayTag::RequestGameplayTag("SetByCaller.Arcade.DamageReduction");
            static const FGameplayTag KnockBackResistanceTag = FGameplayTag::RequestGameplayTag("SetByCaller.Arcade.KnockBackResistance");

            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MaxHealthTag, 
                Data->DifficultyCurve_MaxHealth->GetFloatValue(CurrentWave));
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BaseDamageTag, 
                Data->DifficultyCurve_BaseDamage->GetFloatValue(CurrentWave));
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageReductionTag, 
                Data->DifficultyCurve_DamageReduction->GetFloatValue(CurrentWave));
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, KnockBackResistanceTag, 
                Data->DifficultyCurve_KnockBackResistance->GetFloatValue(CurrentWave));

            ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void UVyraWaveManagerComponent::OnEnemyKilled_Implementation(AVyraEnemyCharacter* Enemy)
{
    if (!Enemy) return;

    SpawnedEnemies.Remove(Enemy);
    
    if (UEnemyDataAsset* EnemyData = GetEnemyDataForType(Enemy->GetEnemyType()))
    {
        CurrentWaveExp += EnemyData->ExperienceValue;
        OnWaveExperienceUpdated.Broadcast(CurrentWaveExp, WaveExpRequirement);

        if (CurrentWaveExp >= WaveExpRequirement)
        {
            EndWave();
        }
    }
}