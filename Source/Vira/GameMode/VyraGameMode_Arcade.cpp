// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameMode_Arcade.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/AbilitySystem/Abilities/Enemies/VyraGameplayAbility_EnemyDeath.h"
#include "Vira/NPC/Enemy/EnemyDataAsset.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"

AVyraGameMode_Arcade::AVyraGameMode_Arcade(): WaveExpCurve(nullptr), WaveSpawnRadius(500.f), BasicEnemies(nullptr),
                                              EliteEnemies(nullptr),
                                              BossEnemies(nullptr),
                                              ArchbossEnemies(nullptr),
                                              VyraPlayerStateCharacter(nullptr),
                                              NavigationSystem(nullptr)
{
}

void AVyraGameMode_Arcade::BeginPlay()
{
	Super::BeginPlay();

	StartNextWave();
}


void AVyraGameMode_Arcade::StartNextWave()
{
	CurrentWave++;
	
	WaveExpRequirement = CalculateWaveExpRequirement(CurrentWave); 

	CurrentWaveExp = 0.0f;

	// You might want to introduce a delay or transition here before starting the wave
	// ...

	SpawnEnemiesForWave();

	// ... Start wave timer, etc. ...
}

void AVyraGameMode_Arcade::EndWave()
{
	ClearRemainingEnemies();
}

void AVyraGameMode_Arcade::OnEnemyKilled_Implementation(TSubclassOf<AVyraEnemyCharacter> Enemy)
{
	// Find the UEnemyData associated with the killed enemy
	if (UEnemyDataAsset* EnemyData = GetEnemyDataForClass(Enemy->GetClass()))
	{
		// Grant experience
		CurrentWaveExp += EnemyData->ExperienceValue;

		// Check if wave completion threshold is reached
		if (CurrentWaveExp >= WaveExpRequirement)
		{
			EndWave();
		}

		// ... (Update UI to show current wave progress) ...
	}
}

void AVyraGameMode_Arcade::SpawnEnemiesForWave()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned null in SpawnEnemies"));
		return;
	}

	if (!VyraPlayerStateCharacter)
	{
		VyraPlayerStateCharacter = UVyraBlueprintFunctionLibrary::GetVyraPlayerCharacter(this);
	}
	if (!NavigationSystem)
	{
		NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	}

	//TODO: Calculate CurrentWaveSpawnRate
	
	World->GetTimerManager().SetTimer(WaveTimerHandle,this,&AVyraGameMode_Arcade::SpawnEnemy, CurrentWaveSpawnRate, true, 0.f);
}

EVyraEnemyType AVyraGameMode_Arcade::DetermineEnemyTypeToSpawn()
{
	// Implement logic to determine the type of enemy to spawn based on wave number, difficulty, etc.
	// Example: Higher chance of basic enemies in early waves, increasing chance of elite/boss/archboss in later waves
	float RandomValue = FMath::FRand();

	if (CurrentWave < 5)
	{
		// Early waves: mostly basic, some elite
		if (RandomValue < 0.8f) return EVyraEnemyType::NORMAL; // 80% chance for basic
		else return EVyraEnemyType::ELITE; // 20% chance for elite
	}
	else if (CurrentWave < 10)
	{
		// Mid waves: mix of basic, elite, some boss
		if (RandomValue < 0.6f) return EVyraEnemyType::NORMAL; // 60% chance for basic
		else if (RandomValue < 0.9f) return EVyraEnemyType::ELITE; // 30% chance for elite
		else return EVyraEnemyType::BOSS; // 10% chance for boss
	}
	else
	{
		// Later waves: basic, elite, boss, some archboss
		if (RandomValue < 0.4f) return EVyraEnemyType::NORMAL; // 40% chance for basic
		else if (RandomValue < 0.7f) return EVyraEnemyType::ELITE; // 30% chance for elite
		else if (RandomValue < 0.9f) return EVyraEnemyType::BOSS; // 20% chance for boss
		else return EVyraEnemyType::ARCHBOSS; // 10% chance for archboss
	}
}

UEnemyDataAsset* AVyraGameMode_Arcade::SelectEnemyToSpawn(EVyraEnemyType EnemyType)
{
	UEnemyDataAsset* SelectedEnemyData = nullptr;

	// Select the appropriate data asset based on EnemyType
	switch (EnemyType)
	{
	case EVyraEnemyType::NORMAL:
		SelectedEnemyData = BasicEnemies;
		break;
	case EVyraEnemyType::ELITE:
		SelectedEnemyData = EliteEnemies;
		break;
	case EVyraEnemyType::BOSS:
		SelectedEnemyData = BossEnemies;
		break;
	case EVyraEnemyType::ARCHBOSS:
		SelectedEnemyData = ArchbossEnemies;
		break;
	default:
		return nullptr;
	}

	if (!SelectedEnemyData)
	{
		UE_LOG(LogTemp, Warning, TEXT("No EnemyDataAsset found for enemy type %s!"), *UEnum::GetValueAsString(EnemyType));
		return nullptr;
	}

	// Check if there are any enemy classes in the array
	if (SelectedEnemyData->EnemyClass.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No EnemyClass found for enemy type %s!"), *UEnum::GetValueAsString(EnemyType));
		return nullptr;
	}

	// Select a random enemy class from the array
	int32 RandomIndex = FMath::RandRange(0, SelectedEnemyData->EnemyClass.Num() - 1);
	TSubclassOf<AVyraEnemyCharacter> SelectedEnemyClass = SelectedEnemyData->EnemyClass[RandomIndex];

	// Now we need to find a UEnemyDataAsset that contains this specific class.
	// We'll iterate through all available UEnemyDataAsset instances to find a match.
	// Note: This is a workaround. Ideally, you should have a direct mapping or a better way to associate data with the selected class.

	for (TObjectIterator<UEnemyDataAsset> It; It; ++It)
	{
		UEnemyDataAsset* EnemyData = *It;
		if (EnemyData && EnemyData->EnemyClass.Contains(SelectedEnemyClass))
		{
			return EnemyData;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find UEnemyDataAsset for the selected enemy class."));
	return nullptr;
}

void AVyraGameMode_Arcade::SpawnEnemy()
{
    // Return if required things aren't valid.
    if (!VyraPlayerStateCharacter || !NavigationSystem) return;

    const FVector SpawnCenter = VyraPlayerStateCharacter->GetActorLocation();

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int i = 0; i < EnemiesToSpawnThisWave; ++i)
	{
		// Determine enemy type to spawn based on some logic
		EVyraEnemyType EnemyTypeToSpawn = DetermineEnemyTypeToSpawn();

		// Select a random enemy of the chosen type
		UEnemyDataAsset* SelectedEnemyData = SelectEnemyToSpawn(EnemyTypeToSpawn);

		if (SelectedEnemyData)
		{
			// Get the array of enemy classes from the selected data asset
			TArray<TSubclassOf<AVyraEnemyCharacter>>& EnemyClasses = SelectedEnemyData->EnemyClass;

			// Check if the array is empty
			if (EnemyClasses.Num() == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("Selected EnemyDataAsset has no EnemyClass entries!"));
				return;
			}

			// Select a random enemy class from the array
			int32 RandomIndex = FMath::RandRange(0, EnemyClasses.Num() - 1);
			TSubclassOf<AVyraEnemyCharacter> SelectedEnemyClass = EnemyClasses[RandomIndex];

			FNavLocation RandomNavLocation;
			FVector RandomSpawnLocation;
			if (NavigationSystem->GetRandomReachablePointInRadius(SpawnCenter, WaveSpawnRadius, RandomNavLocation))
			{
				RandomSpawnLocation = RandomNavLocation.Location;
			}
			else
			{
				RandomSpawnLocation = SpawnCenter;
			}

			FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(RandomSpawnLocation, SpawnCenter);

			// Spawn the enemy
			if (AVyraEnemyCharacter* SpawnedCharacter = GetWorld()->SpawnActor<AVyraEnemyCharacter>(SelectedEnemyClass, RandomSpawnLocation, SpawnRotation, SpawnParameters))
			{
				SpawnedCharacter->OnEnemyKilled.AddDynamic(this, &AVyraGameMode_Arcade::OnEnemyKilled);
				// TODO: Apply gameplay effect to scale wave difficulties.
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn Arcade enemy!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to select enemy data for spawning!"));
		}
	}
}

void AVyraGameMode_Arcade::ClearRemainingEnemies(bool ActivateDeathAbility)
{
	for (AVyraEnemyCharacter* Enemy : SpawnedEnemies)
	{
		if (Enemy)
		{
			if (ActivateDeathAbility)
			{
				Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(UVyraGameplayAbility_EnemyDeath::StaticClass());
			}
			else
			{
				Enemy->Destroy();
			}
		}
	}
}

UEnemyDataAsset* AVyraGameMode_Arcade::GetEnemyDataForClass(TSubclassOf<AVyraEnemyCharacter> EnemyClass)
{
	// Helper function to find the UEnemyData for a given enemy class

	// Iterate through all available UEnemyDataAsset instances
	for (TObjectIterator<UEnemyDataAsset> It; It; ++It)
	{
		UEnemyDataAsset* EnemyData = *It;
		if (EnemyData && EnemyData->EnemyClass.Contains(EnemyClass))
		{
			return EnemyData;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find UEnemyDataAsset for the specified enemy class."));
	return nullptr;
}

float AVyraGameMode_Arcade::CalculateWaveExpRequirement(int32 WaveNumber)
{
	if (WaveExpCurve)
	{
		return WaveExpCurve->GetFloatValue(WaveNumber);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WaveExpCurve not set in Game Mode!"));
		return 100.0f; // Default value if the curve is not set
	}
}