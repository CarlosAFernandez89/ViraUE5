// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_BloodScythe.h"

#include "AbilitySystemBlueprintLibrary.h"


FGameplayAbilityTargetDataHandle UArcadeAbility_BloodScythe::PerformConeTrace(TArray<FHitResult>& OutHits)
{
	const float ConeAngleRadians = FMath::DegreesToRadians(ConeAngleDegrees);

	if (ACharacter* OwningCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		constexpr int32 NumTraces = 10;
		const FVector OriginLocation = OwningCharacter->GetActorLocation();
		const FVector ForwardDirection = OwningCharacter->GetControlRotation().Vector();

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwningCharacter);

		TArray<AActor*> OverlappingActors;

		// Half-angle for left/right sweep of the cone
		const float HalfConeAngle = ConeAngleRadians / 2;
		
		for (int i = 0; i <= NumTraces; i++)
		{
			// Calculate angle for this trace
			float AngleStep = FMath::Lerp(-HalfConeAngle, HalfConeAngle, static_cast<float>(i) / static_cast<float>(NumTraces));

			// Rotate the forward direction to get the trace direction
			FVector TraceDirection = ForwardDirection.RotateAngleAxis(FMath::RadiansToDegrees(AngleStep), FVector::UpVector);

			// Calculate the trace end point
			FVector TraceEnd = OriginLocation + (TraceDirection * ConeLength);

			// Perform a multi-sweep (gets all hits along the path)
			TArray<FHitResult> Hits;
			bool bHit = GetWorld()->SweepMultiByChannel(
				Hits,
				OriginLocation,
				TraceEnd,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(50.0f), // Adjust sphere radius
				QueryParams
			);

			if (bShowDebug)
			{
				// Visualize the cone trace with a line
				DrawDebugLine(GetWorld(), OriginLocation, TraceEnd, FColor::Green, false, 1.0f, 0, 1.0f);

				// Visualize the sphere at the trace end (hit or no hit)
				DrawDebugSphere(GetWorld(), TraceEnd, 50.0f, 12, FColor::Blue, false, 1.0f);
			}
			
			if (bHit)
			{
				for (auto Hit : Hits)
				{
					// Add the hit to the output array
					OverlappingActors.AddUnique(Hit.GetActor());
					OutHits.Add(Hit);

					if (bShowDebug)
					{
						// If we hit something, visualize the hit point
						DrawDebugPoint(GetWorld(), Hit.Location, 10.0f, FColor::Red, false, 1.0f);

						// Log the name of the actor that was hit (for debugging)
						if (const AActor* HitActor = Hit.GetActor())
						{
							UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
						}
					}	
				}
			}
		}
		return UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(OverlappingActors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}
