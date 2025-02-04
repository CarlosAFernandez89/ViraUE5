// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackZone.h"

#include "Components/BoxComponent.h"
#include "Components/GSCCoreComponent.h"


// Sets default values
AAttackZone::AAttackZone()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
}

void AAttackZone::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwningActor = GetOwner())
	{
		if (UGSCCoreComponent* CoreComponent = OwningActor->GetComponentByClass<UGSCCoreComponent>())
		{
			// We deactivate the collision if the owner dies mid attack.
			CoreComponent->OnDeath.AddDynamic(this, &AAttackZone::DeactivateZone);
		}
	}

	ActivateZone();
}

void AAttackZone::ActivateZone()
{
	//Set timer to call OnWindUpFinished after the WindUpTime.
	GetWorldTimerManager().SetTimer(ActivateTimerHandle, this, &AAttackZone::OnWindUpFinished, WindUpTime, false);
}

void AAttackZone::DeactivateZone()
{
	Destroy();
}

void AAttackZone::OnWindUpFinished()
{
	if (OverlapActorClass)
	{
		TArray<AActor*> OverlappingActors;
		CollisionComponent->GetOverlappingActors(OverlappingActors, OverlapActorClass);

		// Broadcast the delegate, passing the array of actors
		ActorsHitByZone(OverlappingActors);
	}
	
	GetWorldTimerManager().SetTimer(DeactivateTimerHandle, this, &AAttackZone::DeactivateZone, ActiveDuration, false);


}


AMeshAttackZone::AMeshAttackZone()
{
	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	ZoneMesh->SetupAttachment(RootComponent);
	ZoneMesh->SetCollisionProfileName("OverlapAllDynamic");
}

void AMeshAttackZone::ActivateZone()
{
	Super::ActivateZone();
	
	// Handle mesh scaling/animation
	MeshMaterial = ZoneMesh->CreateDynamicMaterialInstance(0);
}

void AMeshAttackZone::OnWindUpFinished()
{
	if (OverlapActorClass)
	{
		TArray<AActor*> Return;
		ZoneMesh->GetOverlappingActors(Return, OverlapActorClass);
		ActorsHitByZone(Return);
	}

	GetWorldTimerManager().SetTimer(DeactivateTimerHandle, this, &AAttackZone::DeactivateZone, ActiveDuration, false);

}
