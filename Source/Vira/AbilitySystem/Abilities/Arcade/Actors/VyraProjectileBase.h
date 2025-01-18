// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "VyraProjectileBase.generated.h"

class UGameplayEffect;

UCLASS()
class VIRA_API AVyraProjectileBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Projectile", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Projectile", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
public:
	// Sets default values for this actor's properties
	AVyraProjectileBase();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitWorldObject(const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitActor(AActor* HitActor, const FHitResult& SweepResult);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ACharacter> TargetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = true))
	float LifeSpan = 1.5f;

};
