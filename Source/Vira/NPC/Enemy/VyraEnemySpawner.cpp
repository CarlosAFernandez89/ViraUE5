// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemySpawner.h"

#include "VyraEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AVyraEnemySpawner::AVyraEnemySpawner(): SpawnRate(2), SpawnBoundingBoxHalfSize(FVector(400.f, 400.f, 50.f)),
                                        SpawnLocation(),
                                        SpawnRotation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AVyraEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();
	SpawnEnemies_Implementation();
}

void AVyraEnemySpawner::SpawnEnemiesOfType(const TSubclassOf<AVyraEnemyCharacter>& EnemyClass, const int Quantity)
{
	if (EnemyClass)
	{
		//FActorSpawnParameters SpawnParameters;
		//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		for (int i = 0; i < Quantity; i++)
		{
			FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
				SpawnLocation,
				FVector(SpawnBoundingBoxHalfSize.X, SpawnBoundingBoxHalfSize.Y, SpawnBoundingBoxHalfSize.Z)
				);

			if (UWorld* World = GetWorld())
			{
				World->SpawnActor<AVyraEnemyCharacter>(EnemyClass, RandomLocation, SpawnRotation);
			}
		}
	}
}

void AVyraEnemySpawner::SpawnEnemies_Implementation()
{
	if (const UWorld* World = GetWorld())
	{
		float SpawnTime = SpawnRate;

		for (int i = 0; i < SpawnerData.Num(); ++i)
		{		
			TSubclassOf<AVyraEnemyCharacter> EnemyClass = SpawnerData[i].EnemyType;
			int Quant = SpawnerData[i].Quantity;
		
			FTimerHandle TempTimerHandle;
			World->GetTimerManager().SetTimer(
				TempTimerHandle,[this, EnemyClass, Quant]()
				{
					SpawnEnemiesOfType(EnemyClass, Quant);
				},SpawnTime,false);
		
			SpawnTime += SpawnRate;
		}
	}
}
