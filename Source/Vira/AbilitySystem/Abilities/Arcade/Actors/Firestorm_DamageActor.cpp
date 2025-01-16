// Fill out your copyright notice in the Description page of Project Settings.


#include "Firestorm_DamageActor.h"

#include "Components/SphereComponent.h"


// Sets default values
AFirestorm_DamageActor::AFirestorm_DamageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereCollisionComponent->SetMobility(EComponentMobility::Static);
	SphereCollisionComponent->InitSphereRadius(FirestormRadius);
	
	RootComponent = SphereCollisionComponent;

}

void AFirestorm_DamageActor::BeginPlay()
{
	Super::BeginPlay();

	// We must set it in case the radius changes due to other abilities.
	SphereCollisionComponent->SetSphereRadius(FirestormRadius);
}

void AFirestorm_DamageActor::StartDamageEvent_Implementation()
{
	SetLifeSpan(FirestormDuration);
}

void AFirestorm_DamageActor::AbilityCastFailed_Implementation()
{
}

