// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityTargetActor_MouseLocationTrace.h"

#include "Abilities/GameplayAbility.h"

FHitResult AGameplayAbilityTargetActor_MouseLocationTrace::PerformTrace(AActor* InSourceActor)
{
	PlayerController = OwningAbility->GetCurrentActorInfo()->PlayerController.Get();
	check(PlayerController);

	FHitResult HitResult;
	GetMouseLocation(InSourceActor, HitResult, 50000.f);

	return HitResult;
}

bool AGameplayAbilityTargetActor_MouseLocationTrace::GetMouseLocation(const AActor* InSourceActor, FHitResult& HitResult, float TraceDistance) const
{
	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		const FVector Start = WorldLocation;
		const FVector End = Start + (WorldDirection * TraceDistance);
		
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(InSourceActor);

		// Perform the line trace
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel4, CollisionParams))
		{
			// Adjust the hit location to be within MaxDistance from InSourceActor
			FVector SourceToHit = HitResult.Location - InSourceActor->GetActorLocation();
			if (SourceToHit.Size() > MaxRange)
			{
				SourceToHit = SourceToHit.GetSafeNormal() * MaxRange;
				HitResult.Location = InSourceActor->GetActorLocation() + SourceToHit;

				// Optionally, update the ImpactPoint as well
				HitResult.ImpactPoint = HitResult.Location; 
			}
			
			return true;
		}
	}
	return false;
}

