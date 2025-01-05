// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VyraEnemySpawner.generated.h"


class AVyraEnemyCharacter;

USTRUCT(BlueprintType)
struct FEnemySpawnerData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVyraEnemyCharacter> EnemyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity;

};


UCLASS()
class VIRA_API AVyraEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVyraEnemySpawner();

	UFUNCTION(BlueprintNativeEvent)
	void SpawnEnemies();

protected:
	virtual void BeginPlay() override;
	
private:
	void SpawnEnemiesOfType(const TSubclassOf<AVyraEnemyCharacter>& EnemyClass, int Quantity);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TArray<FEnemySpawnerData> SpawnerData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	FVector SpawnBoundingBoxHalfSize;

	private:
	FVector SpawnLocation;
	FRotator SpawnRotation;
};
