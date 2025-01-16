// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "GameplayAbilityTargetActor_MouseLocationTrace.generated.h"

UCLASS()
class VIRA_API AGameplayAbilityTargetActor_MouseLocationTrace : public AGameplayAbilityTargetActor_SingleLineTrace
{
	GENERATED_BODY()

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;

	UFUNCTION(BlueprintCallable)
	bool GetMouseLocation(const AActor* InSourceActor, FHitResult& HitResult, float TraceDistance) const;

private:
	UPROPERTY()
	APlayerController* PlayerController;
};
