// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "VyraProjectileBase.generated.h"

class UNiagaraSystem;
class UVyraAbilitySystemComponent;
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

	void SetProjectileSpeed(float InProjectileSpeed = 850.f) const;
	void UpdateProjectileSpeed(float InSpeedMultiplier = 1.f) const;
	void SetupProjectileOwnerValues(ACharacter* InOwningCharacter, UVyraAbilitySystemComponent* InOwningASC, bool InShouldFork = true);
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnHitWorldObject(const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnHitActor(AActor* HitActor, const FHitResult& SweepResult);

private:
	UFUNCTION()
	void SpawnVisuals();

	UFUNCTION()
	float CalculateLifeSpanLinear() const;
	
	UFUNCTION()
	bool ShouldProjectileFork() const;

	UFUNCTION()
	void ForkProjectile(AActor* ForkInstigator) const;

	UFUNCTION()
	bool IsProjectilePhasing() const;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ACharacter> TargetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (ExposeOnSpawn = true))
	float LifeSpan = 1.5f;

	UPROPERTY()
	AActor* ProjectileInstigator;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	FGameplayTag ProjectilePhasingTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Visuals")
	UNiagaraSystem* ProjectileNiagaraSystem;
	
private:
	UPROPERTY()
	ACharacter* OwningCharacter;

	UPROPERTY()
	UVyraAbilitySystemComponent* OwnerASC;

	UPROPERTY()
	bool bAlreadyForked = false;
};
