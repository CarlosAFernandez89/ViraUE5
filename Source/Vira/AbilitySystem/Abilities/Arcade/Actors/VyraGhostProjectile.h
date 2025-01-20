// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VyraGhostProjectile.generated.h"

class UNiagaraSystem;

UCLASS()
class VIRA_API AVyraGhostProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Projectile", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* ProjectileNiagaraComponent;


public:
	// Sets default values for this actor's properties
	AVyraGhostProjectile();

	void InitializeNiagaraSystem(UNiagaraSystem* NiagaraSystem) const;

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateProjectileLocation();
};
