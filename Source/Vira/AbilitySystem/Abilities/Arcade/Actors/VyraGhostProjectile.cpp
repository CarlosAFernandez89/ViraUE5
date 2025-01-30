// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGhostProjectile.h"

#include "NiagaraComponent.h"


// Sets default values
AVyraGhostProjectile::AVyraGhostProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ProjectileNiagaraComponent");
	RootComponent = ProjectileNiagaraComponent;
}

void AVyraGhostProjectile::InitializeNiagaraSystem(UNiagaraSystem* NiagaraSystem) const
{
	if (NiagaraSystem)
	{
		ProjectileNiagaraComponent->SetAsset(NiagaraSystem);
	}
}


// Called every frame
void AVyraGhostProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateProjectileLocation();
}

void AVyraGhostProjectile::UpdateProjectileLocation()
{
	if (GetOwner())
	{
		SetActorLocation(GetOwner()->GetActorLocation());

		if (GetOwner()->IsActorBeingDestroyed())
		{
			SetActorTickEnabled(false);
			ProjectileNiagaraComponent->Deactivate();
			SetLifeSpan(0.5f);
		}
	}
	else
	{
		SetActorTickEnabled(false);
		ProjectileNiagaraComponent->Deactivate();
		SetLifeSpan(0.5f);
	}
}