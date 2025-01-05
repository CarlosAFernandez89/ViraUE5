// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemySpawner.h"

#include "VyraEnemyCharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AVyraEnemySpawner::AVyraEnemySpawner(): SpawnRate(2), SpawnBoundingBoxHalfSize(FVector(400.f, 400.f, 50.f))
{

	//Make the root component static so it has a transform
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(RootComponent);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AVyraEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemies();
}

void AVyraEnemySpawner::SpawnEnemiesOfType()
{
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		if (SpawnIteration < SpawnerData.Num())
		{
			for (int j = 0; j < SpawnerData[SpawnIteration].Quantity; j++)
			{
				FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
					SpawnLocation,
					FVector(SpawnBoundingBoxHalfSize.X, SpawnBoundingBoxHalfSize.Y, SpawnBoundingBoxHalfSize.Z)
					);

				World->SpawnActor<AVyraEnemyCharacter>(SpawnerData[SpawnIteration].EnemyType, RandomLocation, SpawnRotation, SpawnParameters);			
			}
			++SpawnIteration;
		}
		else
		{
			GetWorldTimerManager().ClearAllTimersForObject(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is null in function SpawnEnemies"));
	}
}

void AVyraEnemySpawner::SpawnEnemies()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned null in SpawnEnemies"));
		return;
	}

	TArray<FEnemySpawnerData> Data = SpawnerData;
	FTimerHandle TempTimerHandle;
	World->GetTimerManager().SetTimer(
		TempTimerHandle,
		this ,
		&AVyraEnemySpawner::SpawnEnemiesOfType,
		SpawnRate, true, SpawnRate);
		
}
