// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraGameplayAbility.h"
#include "VyraGameplayAbility_Companion.generated.h"

class UBehaviorTree;
class AVyraCompanionPawn;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_Companion : public UVyraGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnCompanionPawn();

	UFUNCTION(BlueprintCallable)
	void DestroyCompanionPawn();

	UFUNCTION(BlueprintCallable, Category = "VyraGameplayAbility|Companion")
	bool IsCompanionSpawned() const
	{
		return CompanionPawnReference ? true : false;
	}
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyra|Ability|Companion")
	bool bSpawnOnGiveAbility = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyra|Ability|Companion")
	TSubclassOf<AVyraCompanionPawn> CompanionPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyra|Ability|Companion")
	float SpawnRadius = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vyra|Ability|Companion")
	UBehaviorTree* CompanionPawnBehavior;


private:

	UPROPERTY()
	AVyraCompanionPawn* CompanionPawnReference;
};
