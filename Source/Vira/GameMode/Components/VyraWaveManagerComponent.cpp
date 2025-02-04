// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWaveManagerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Curves/CurveVector.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/AbilitySystem/Abilities/Enemies/VyraGameplayAbility_EnemyDeath.h"
#include "Vira/NPC/Enemy/EnemyDataAsset.h"
#include "Vira/NPC/Enemy/EnemySpawnPoint.h"
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

    CollectSpawnPoints();
    
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
    if (SpawnPoints.Num() == 0) return;

    TryClearingNullArrayItems();

    const int32 RemainingEnemies = MaxEnemiesToSpawnThisWave - SpawnedEnemies.Num();
    if (RemainingEnemies <= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
        return;
    }

    const int32 EnemiesToSpawnNow = FMath::Min(GetEnemiesToSpawn(CurrentWave), RemainingEnemies);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < EnemiesToSpawnNow; ++i)
    {
        FVector SpawnLocation;
        FRotator SpawnRotation;
        
        if (!GetSpawnTransform(SpawnLocation, SpawnRotation)) continue;

        const EVyraEnemyType EnemyType = DetermineEnemyTypeToSpawn();
        if (UEnemyDataAsset* EnemyData = SelectEnemyToSpawn(EnemyType))
        {
            TSubclassOf<AVyraEnemyCharacter> EnemyClass = EnemyData->EnemyClass[FMath::RandRange(0, EnemyData->EnemyClass.Num() - 1)];
            
            if (AVyraEnemyCharacter* Enemy = GetWorld()->SpawnActor<AVyraEnemyCharacter>(
                EnemyClass,
                SpawnLocation,
                SpawnRotation,
                SpawnParams))
            {
                Enemy->SetActorEnableCollision(true);
                Enemy->GetMesh()->SetSimulatePhysics(false);
                Enemy->OnEnemyKilled.AddDynamic(this, &UVyraWaveManagerComponent::OnEnemyKilled);
                SpawnedEnemies.Add(Enemy);
                ApplyScalingGameplayEffect(Enemy, Enemy->GetEnemyType());
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

void UVyraWaveManagerComponent::CollectSpawnPoints()
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AEnemySpawnPoint::StaticClass(), FoundActors);

    SpawnPoints.Empty();
    for (AActor* Actor : FoundActors)
    {
        if (AEnemySpawnPoint* SpawnPoint = Cast<AEnemySpawnPoint>(Actor))
        {
            SpawnPoints.Add(SpawnPoint);
        }
    }
}

bool UVyraWaveManagerComponent::GetSpawnTransform(FVector& OutLocation, FRotator& OutRotation)
{
    if (SpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No spawn points found!"));
        return false;
    }

    // Select random spawn point
    const int32 PointIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
    const AEnemySpawnPoint* SpawnPoint = SpawnPoints[PointIndex];
    const FVector SpawnCenter = SpawnPoint->GetActorLocation();

    // Find valid spawn location
    FNavLocation NavLocation;
    const bool bFound = NavigationSystem->GetRandomReachablePointInRadius(
        SpawnCenter,
        SpawnPoint->SpawnRadiusMax,
        NavLocation
    );

    if (!bFound) return false;

    // Validate minimum distance
    const float Distance = FVector::Dist2D(NavLocation.Location, SpawnCenter);
    if (Distance < SpawnPoint->SpawnRadiusMin)
    {
        const FVector Direction = (NavLocation.Location - SpawnCenter).GetSafeNormal2D();
        NavLocation.Location = SpawnCenter + Direction * SpawnPoint->SpawnRadiusMin;
        NavigationSystem->ProjectPointToNavigation(NavLocation.Location, NavLocation);
    }

    // Ground check
    FVector GroundLocation;
    if (!FindGroundPosition(NavLocation.Location, GroundLocation)) return false;

    // Rotation
    const AActor* PlayerActor = UGameplayStatics::GetActorOfClass(this, AVyraPlayerStateCharacter::StaticClass());
    OutRotation = SpawnPoint->bRandomizeRotation ? 
        FRotator(0, FMath::RandRange(0.f, 360.f), 0) : 
        UKismetMathLibrary::FindLookAtRotation(GroundLocation, PlayerActor->GetActorLocation());

    OutLocation = GroundLocation;
    return true;
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
