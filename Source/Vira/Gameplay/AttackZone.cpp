// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackZone.h"

#include "Components/GSCCoreComponent.h"


// Sets default values
AAttackZone::AAttackZone()
{
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
	GetWorldTimerManager().SetTimer(UpdateMaterialTimerHandle, this, &AAttackZone::UpdateMaterial, GetWorld()->DeltaTimeSeconds, true);

}

void AAttackZone::DeactivateZone()
{
	Destroy();
}

void AAttackZone::OnWindUpFinished()
{
	GetWorldTimerManager().ClearTimer(UpdateMaterialTimerHandle);
	GetWorldTimerManager().SetTimer(DeactivateTimerHandle, this, &AAttackZone::DeactivateZone, ActiveDuration, false);
}

void AAttackZone::UpdateMaterial_Implementation()
{
	
}

float AAttackZone::CalculatePlayRate() const
{
	return 1.f / WindUpTime;
}

AMeshAttackZone::AMeshAttackZone()
{
	ProgressZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProgressZoneMesh"));
	ProgressZoneMesh->SetupAttachment(RootComponent);
	ProgressZoneMesh->SetCollisionProfileName("OverlapAllDynamic");

	AreaZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaZoneMesh"));
	AreaZoneMesh->SetupAttachment(ProgressZoneMesh);
	AreaZoneMesh->SetCollisionProfileName("NoCollision");
}

void AMeshAttackZone::BeginPlay()
{
	Super::BeginPlay();

	ProgressIndicator = ProgressZoneMesh->CreateDynamicMaterialInstance(0, ProgressZoneMesh->GetMaterial(0));
	ProgressIndicator->SetScalarParameterValue(FName("Progress"), 0.f);
	ProgressIndicator->SetVectorParameterValue(FName("Color"), ZoneColor);

	AreaIndicator = AreaZoneMesh->CreateDynamicMaterialInstance(0, AreaZoneMesh->GetMaterial(0));
	AreaIndicator->SetScalarParameterValue(FName("Border"), 1.f);
	AreaIndicator->SetVectorParameterValue(FName("Color"), ZoneColor);

}

void AMeshAttackZone::ActivateZone()
{
	Super::ActivateZone();
}

void AMeshAttackZone::OnWindUpFinished()
{
	if (OverlapActorClass)
	{
		TArray<AActor*> Return;
		ProgressZoneMesh->GetOverlappingActors(Return, OverlapActorClass);
		OnAttackTriggered(Return);
	}

	GetWorldTimerManager().SetTimer(DeactivateTimerHandle, this, &AAttackZone::DeactivateZone, ActiveDuration, false);

}
