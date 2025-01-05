// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemySpawner.h"

#include "VyraEnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AVyraEnemySpawner::AVyraEnemySpawner(): SpawnRate(2)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AVyraEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemies_Implementation();
}

void AVyraEnemySpawner::SpawnEnemiesOfType(const TSubclassOf<AVyraEnemyCharacter>& EnemyClass, int Quantity)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SpawnEnemiesOfType called."));
	
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(
			SpawnLocation,
			FVector(SpawnBoundingBoxHalfSize.X, SpawnBoundingBoxHalfSize.Y, SpawnBoundingBoxHalfSize.Z));
		const FRotator SpawnRotation = GetActorRotation();
		
		if (UWorld* World = GetWorld())
		{
			for (int i = 0; i < Quantity; i++)
			{
				World->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation,SpawnParameters);
			}
		}
	}
}

void AVyraEnemySpawner::SpawnEnemies_Implementation()
{
	float SpawnTime = SpawnRate;

	for (int i = 0; i < SpawnerData.Num(); ++i)
	{		
		TSubclassOf<AVyraEnemyCharacter> EnemyClass = SpawnerData[i].EnemyType;
		int Quant = SpawnerData[i].Quantity;
		FTimerHandle TempTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TempTimerHandle,
			[this, EnemyClass, Quant]()
			{
				SpawnEnemiesOfType(EnemyClass, Quant);
			},
			SpawnTime,
			false);
		
		SpawnTime += SpawnRate;
	}
}
