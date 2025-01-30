// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IAIHelpers.h"
#include "Vira/NPC/VyraNPCWithASC.h"
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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, AVyraEnemyCharacter*, EnemyClass);


UCLASS()
class VIRA_API AVyraEnemyCharacter : public AVyraNPCWithASC, public IAIHelpers
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<AActor*, int32> ReservedAttackTokens;

	UPROPERTY()
	class AVyraAIController* AIController;
	
public:
	// Sets default values for this character's properties
	AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ESpudRespawnMode GetSpudRespawnMode_Implementation() const override { return ESpudRespawnMode::AlwaysRespawn; };

	UFUNCTION(BlueprintCallable)
	EVyraEnemyType GetEnemyType() const {return EnemyType;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Loot")
	void SpawnDropTableItems(float DropInterval);


	//~ IAIHelpers
	virtual AAIPatrolRoute* GetAIPatrolRoute_Implementation() override;
	virtual float SetAIMovementSpeed_Implementation(EAIMovementSpeed MovementSpeed) override;
	virtual void GetAIIdealRange_Implementation(float& AttackRadius, float& DefendRadius) override;
	virtual float GetAICurrentHealth_Implementation() override;
	virtual float GetAIMaxHealth_Implementation() override;
	virtual bool GetAIIsDead_Implementation() override;
	virtual void AIStoreAttackTokens_Implementation(AActor* AttackTarget, int32 Amount) override;
	virtual void AIReturnAllAttackTokens_Implementation() override;
	virtual void CheckFocusTargetState_Implementation() override;
	virtual void NavLink_JumpToDestination_Implementation(FVector Destination) override;
	//~ End IAIHelpers

private:
	void SpawnItemActor(const TSubclassOf<ACurrencyDropBase>& DropClass, int32 Quantity);

	UFUNCTION()
	void OnGSCDeathEvent();
	
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

	bool bHasFocusTarget;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Patrol")
	AAIPatrolRoute* PatrolRoute = nullptr;

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FOnEnemyKilled OnEnemyKilled;
};
