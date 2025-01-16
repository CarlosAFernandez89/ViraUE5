// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_InfernalDash.h"

#include "Vira/AbilitySystem/Abilities/Arcade/Actors/InfernalDash_TrailDamageActor.h"

void UArcadeAbility_InfernalDash::PerformTeleport()
{
	if (ACharacter* OwningActor = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		const FVector StartLocation = OwningActor->GetActorLocation();
		const FVector Direction = OwningActor->GetControlRotation().Vector();
		FVector EndLocation = StartLocation + Direction * TeleportDistance;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwningActor);

		if (GetWorld()->LineTraceSingleByChannel(
				HitResult,         // The result of the trace will be stored here
				StartLocation,     // Start of the ray
				EndLocation,       // End of the ray
				ECC_Visibility,    // Collision channel
				QueryParams        // Additional parameters
			))
		{
			EndLocation = HitResult.Location;
		}

		OwningActor->SetActorLocation(EndLocation, true);
		
		OnTeleportEnd.Broadcast(EndLocation);
		
		if (bSpawnFireTrail)
		{
			SpawnFireTrail(StartLocation, EndLocation);
		}
	}
}

void UArcadeAbility_InfernalDash::SpawnFireTrail(const FVector& StartLocation, const FVector& EndLocation)
{
	const float Distance = FVector::Dist(StartLocation, EndLocation);
	const int32 NumberOfActorsToSpawn = FMath::CeilToInt(Distance / TrailDamageSpawnInterval);

	const FVector Direction = (EndLocation - StartLocation).GetSafeNormal();


	// We only need to modify its spawn height once since the game doesn't have verticality.
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());
	FVector RayStartLocation = StartLocation;

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		RayStartLocation,
		RayStartLocation - (FVector::UpVector * 500.f),
		ECC_Visibility,
		QueryParams))
	{
		RayStartLocation = HitResult.Location;
	}

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(TrailDamageEffectClass, GetAbilityLevel());
	
	for (int i = 1; i <= NumberOfActorsToSpawn; i++)
	{
		const FVector SpawnLocation = RayStartLocation + (Direction * (i * TrailDamageSpawnInterval));
		
		if (AInfernalDash_TrailDamageActor* SpawnedActor = GetWorld()->SpawnActor<AInfernalDash_TrailDamageActor>(TrailDamageActorClass, SpawnLocation, FRotator::ZeroRotator))
		{
			SpawnedActor->SetLifeSpan(TrailDamageLifeSpan);
			SpawnedActor->SetDamageInterval(TrailDamage_DamageInterval);
			SpawnedActor->SetGameplayEffectSpecHandle(SpecHandle);
		}
	}
}
