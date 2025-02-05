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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	float WindUpTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	float ActiveDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack Zone", meta = (ExposeOnSpawn = true))
	FLinearColor ZoneColor = FColor::Red;
	
	
	UFUNCTION(BlueprintCallable)
	virtual void ActivateZone();
	
	UFUNCTION(BlueprintCallable)
	virtual void DeactivateZone();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackTriggered(const TArray<AActor*>& HitActors);

protected:

	UFUNCTION()
	virtual void OnWindUpFinished();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMaterial();
	
	UFUNCTION(BlueprintCallable)
	float CalculatePlayRate() const;

	FTimerHandle UpdateMaterialTimerHandle;
	FTimerHandle ActivateTimerHandle;
	FTimerHandle DeactivateTimerHandle;

private:
};

UCLASS()
class VIRA_API AMeshAttackZone : public AAttackZone
{
	GENERATED_BODY()

public:
	AMeshAttackZone();

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* ProgressZoneMesh;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UMaterialInstanceDynamic* ProgressIndicator;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* AreaZoneMesh;
	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UMaterialInstanceDynamic* AreaIndicator;

	virtual void ActivateZone() override;

	virtual void OnWindUpFinished() override;
};
