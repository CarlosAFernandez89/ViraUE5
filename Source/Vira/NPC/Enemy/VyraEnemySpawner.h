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
	int Quantity = 0;

};


UCLASS(Blueprintable)
class VIRA_API AVyraEnemySpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Default)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = Default)
	class UBillboardComponent* BillboardComponent;
public:
	// Sets default values for this actor's properties
	AVyraEnemySpawner();

	UFUNCTION()
	void SpawnEnemies();

protected:
	virtual void BeginPlay() override;
	
private:
	
	UFUNCTION()
	void SpawnEnemiesOfType();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TArray<FEnemySpawnerData> SpawnerData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	FVector SpawnBoundingBoxHalfSize;

	UPROPERTY()
	int32 SpawnIteration = 0;
};
