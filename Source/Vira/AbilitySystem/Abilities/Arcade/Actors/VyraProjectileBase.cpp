// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraProjectileBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"


// Sets default values
AVyraProjectileBase::AVyraProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(32.f);
	SphereComponent->SetEnableGravity(false);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVyraProjectileBase::SphereComponentBeginOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 850.f;
	ProjectileMovementComponent->MaxSpeed = 850.f;
	ProjectileMovementComponent->Velocity = FVector(1.f, 0.f, 0.f);
}

void AVyraProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
}

void AVyraProjectileBase::SphereComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                                     AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                     const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(TargetClass))
	{
		if (!OtherActor->IsA(AVyraPlayerStateCharacter::StaticClass()))
		{
			OnHitWorldObject(SweepResult.ImpactPoint);
		}
		return;
	}

	if (!DamageEffect.IsValid()) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor, true))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());
	}

	OnHitActor(OtherActor);
}

