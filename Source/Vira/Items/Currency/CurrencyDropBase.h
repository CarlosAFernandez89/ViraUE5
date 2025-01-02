﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/Actor.h"
#include "Vira/System/SaveGame/VyraSPUDActorRuntime.h"
#include "CurrencyDropBase.generated.h"

UCLASS()
class VIRA_API ACurrencyDropBase : public AVyraSPUDActorRuntime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	/// Because this is a runtime-spawned object, every projectile needs a unique identifier
	/// so that they can be re-spawned on load, Naming aproperty "SpudGuid" does this. We don't need to give
	/// it a value, SPUD does that when saving if it's blank
	UPROPERTY(SaveGame)
	FGuid SpudGuid;

public:
	// Sets default values for this actor's properties
	ACurrencyDropBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency Drop")
	float Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency Drop")
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Currency Drop")
	void OnCurrencyAcquired();

private:
	UPROPERTY()
	class UAbilitySystemComponent* TargetASC;
};
