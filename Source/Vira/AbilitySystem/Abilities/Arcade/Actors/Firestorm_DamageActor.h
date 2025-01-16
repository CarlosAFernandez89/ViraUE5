// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Firestorm_DamageActor.generated.h"

class UGameplayEffect;

UCLASS()
class VIRA_API AFirestorm_DamageActor : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firestorm", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollisionComponent;
	
public:
	// Sets default values for this actor's properties
	AFirestorm_DamageActor();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Firestorm")
	void AbilityCastFailed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category = "Firestorm")
	void StartDamageEvent();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firestorm", meta = (ExposeOnSpawn = true))
	UGameplayAbility* OwnedAbility;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firestorm", meta = (ExposeOnSpawn = true))
	float FirestormRadius = 250.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firestorm", meta = (ExposeOnSpawn = true))
	float FirestormDuration = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firestorm", meta = (ExposeOnSpawn = true))
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Firestorm", meta = (ExposeOnSpawn = true))
	float DamageTickInterval = 0.5f;
};
