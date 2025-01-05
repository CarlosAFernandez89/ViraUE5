// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "GameplayAbility_Look.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class VIRA_API UGameplayAbility_Look : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Look();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Player|Movement")
	void Look(const FVector2D& MovementInput);
	bool RaycastToGround(FVector& HitLocation, float TraceDistance = 10000.f);

	UFUNCTION(BlueprintNativeEvent)
	void DrawMouseLocationToWorld(const FVector InMouseHitLocation);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* MouseLocationNiagaraSystem;
	
private:
	UPROPERTY()
	class UCommonInputSubsystem* CommonInputSubsystem;
	UPROPERTY()
	class APlayerController* PlayerController;
	
	FVector CurrentMouseHitLocation;

};
