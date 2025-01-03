// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/NPC/VyraNPCCharacterBase.h"
#include "VyraEnemyCharacter.generated.h"

struct FCurrencyDropData;
class ACurrencyDropBase;

UENUM(BlueprintType)
enum class EVyraEnemyType : uint8
{
	NORMAL UMETA(DisplayName = "Normal"),
	ELITE UMETA(DisplayName = "Elite"),
	BOSS UMETA(DisplayName = "Boss"),
	ARCHBOSS UMETA(DisplayName = "Archboss")
};

USTRUCT(BlueprintType)
struct FEnemyDropTable
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCurrencyDropData> CurrencyDrops;
};



UCLASS()
class VIRA_API AVyraEnemyCharacter : public AVyraNPCCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ESpudRespawnMode GetSpudRespawnMode_Implementation() const override { return ESpudRespawnMode::AlwaysRespawn; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Loot")
	void SpawnDropTableItems(float DropInterval);

private:
	void SpawnItemActor(const TSubclassOf<ACurrencyDropBase>& DropClass, int32 Quantity);
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Enemy")
	EVyraEnemyType EnemyType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Loot")
	FEnemyDropTable DropTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Loot")
	FVector SpawnItemOffset;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Loot")
	float RotationRollMin = -90.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Loot")
	float RotationRollMax = 90.f;

	UPROPERTY()
	FTimerHandle DropItems_TimerHandle;

	
};
