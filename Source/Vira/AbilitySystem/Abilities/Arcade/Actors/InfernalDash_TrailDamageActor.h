// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "InfernalDash_TrailDamageActor.generated.h"

UCLASS()
class VIRA_API AInfernalDash_TrailDamageActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InfernalDash", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollisionComponent;

protected:

	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	
public:
	// Sets default values for this actor's properties
	AInfernalDash_TrailDamageActor();

	UFUNCTION()
	void SetDamageInterval(const float InDamageInterval) { DamageInterval = InDamageInterval; }

	UFUNCTION()
	void SetGameplayEffectSpecHandle(const FGameplayEffectSpecHandle& InGameplayEffectSpecHandle)
	{
		SpecHandle = InGameplayEffectSpecHandle;
	};
	
private:

	UFUNCTION()
	void ApplyDamage() const;
	
	UPROPERTY()
	FTimerHandle ApplyDamage_TimerHandle;

	UPROPERTY()
	float DamageInterval = 0.5f;

	UPROPERTY()
	FGameplayEffectSpecHandle SpecHandle;
};
