// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "AttackZone.generated.h"

class UBoxComponent;

UCLASS()
class VIRA_API AAttackZone : public AActor
{
	GENERATED_BODY()

public:
	AAttackZone();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	TSubclassOf<AActor> OverlapActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone")
	float WindUpTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone")
	float ActiveDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionComponent;
	
	UFUNCTION(BlueprintCallable)
	virtual void ActivateZone();
	
	UFUNCTION(BlueprintCallable)
	virtual void DeactivateZone();

	UFUNCTION(BlueprintImplementableEvent)
	void ActorsHitByZone(const TArray<AActor*>& HitActors);

protected:

	UFUNCTION()
	virtual void OnWindUpFinished();

	FTimerHandle ActivateTimerHandle;
	FTimerHandle DeactivateTimerHandle;
};

UCLASS()
class VIRA_API AMeshAttackZone : public AAttackZone
{
	GENERATED_BODY()

public:
	AMeshAttackZone();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ZoneMesh;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* MeshMaterial;

	virtual void ActivateZone() override;

	virtual void OnWindUpFinished() override;
};
