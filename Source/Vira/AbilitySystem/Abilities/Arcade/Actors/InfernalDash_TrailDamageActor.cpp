// Fill out your copyright notice in the Description page of Project Settings.


#include "InfernalDash_TrailDamageActor.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Vira/NPC/Enemy/VyraEnemyCharacter.h"


// Sets default values
AInfernalDash_TrailDamageActor::AInfernalDash_TrailDamageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereCollisionComponent->SetMobility(EComponentMobility::Static);
	SphereCollisionComponent->InitSphereRadius(100.f);
	
	RootComponent = SphereCollisionComponent;
	
}

void AInfernalDash_TrailDamageActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ApplyDamage_TimerHandle, this, &AInfernalDash_TrailDamageActor::ApplyDamage,
		DamageInterval, true);
}

void AInfernalDash_TrailDamageActor::Destroyed()
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, ApplyDamage_TimerHandle);

	Super::Destroyed();
}

void AInfernalDash_TrailDamageActor::ApplyDamage()
{
	TArray<AActor*> OverlappingActors;
	SphereCollisionComponent->GetOverlappingActors(OverlappingActors, AVyraEnemyCharacter::StaticClass());

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (const AVyraEnemyCharacter* CastedActor = Cast<AVyraEnemyCharacter>(OverlappingActor))
		{
			if (UAbilitySystemComponent* ASC = CastedActor->GetAbilitySystemComponent())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			}
		}
	}
}



