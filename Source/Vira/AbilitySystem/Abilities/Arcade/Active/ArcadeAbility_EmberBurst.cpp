// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_EmberBurst.h"

#include "Vira/AbilitySystem/Abilities/Arcade/Actors/VyraProjectileBase.h"

void UArcadeAbility_EmberBurst::SpawnProjectilesInCone(const int32 NumProjectiles, const float ConeHalfAngleDegrees)
{
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass in UArcadeAbility_EmberBurst::SpawnProjectilesInCone is not set!"));
		return;
	}

	if (NumProjectiles <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NumProjectiles in UArcadeAbility_EmberBurst::SpawnProjectilesInCone is less than or equal to 0!"));
		return;
	}
	
	float ConeHalfAngleRadians = FMath::DegreesToRadians(ConeHalfAngleDegrees);
	FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector UpVector = GetAvatarActorFromActorInfo()->GetActorUpVector();
	
	float AngleStep = (NumProjectiles > 1) ? (ConeHalfAngleRadians * 2.0f) / (NumProjectiles - 1) : 0.0f;
	float StartAngle = (NumProjectiles > 1) ? - ConeHalfAngleRadians : 0.f;

	const int32 AbilityLevel = GetVyraAbilitySystemComponent()->GetGameplayTagCount(AbilityLevelTag);
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ProjectileDamageEffect, AbilityLevel);

	if (SpecHandle.IsValid())
	{
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			float ProjectileAngle = StartAngle + AngleStep * i;

			FVector ProjectileDirection = ForwardVector.RotateAngleAxis(FMath::RadiansToDegrees(ProjectileAngle), UpVector);

			if (AVyraProjectileBase* Projectile = GetWorld()->SpawnActor<AVyraProjectileBase>(ProjectileClass, SpawnLocation, ProjectileDirection.Rotation()))
			{
				Projectile->SetOwner(GetAvatarActorFromActorInfo());
				Projectile->DamageEffect = SpecHandle;
			}
		}
	}
}
