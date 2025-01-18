// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_EmberBurst.h"

#include "Kismet/GameplayStatics.h"
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

	const float ConeHalfAngleRadians = FMath::DegreesToRadians(ConeHalfAngleDegrees);
	const FVector ForwardVector = PlayerStateCharacter->GetActorForwardVector();
	const FVector SpawnLocation = PlayerStateCharacter->GetActorLocation() + (ForwardVector * ProjectileSpawnOffset);
	const FVector UpVector = PlayerStateCharacter->GetActorUpVector();

	const float AngleStep = (NumProjectiles > 1) ? (ConeHalfAngleRadians * 2.0f) / (NumProjectiles - 1) : 0.0f;
	const float StartAngle = (NumProjectiles > 1) ? - ConeHalfAngleRadians : 0.f;

	const int32 AbilityLevel = GetGameplayTagStackCount(GetAbilityLevelTag());
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(ProjectileDamageEffect, AbilityLevel);

	if (SpecHandle.IsValid())
	{
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			float ProjectileAngle = StartAngle + AngleStep * i;

			FVector ProjectileDirection = ForwardVector.RotateAngleAxis(FMath::RadiansToDegrees(ProjectileAngle), UpVector);

			if (AVyraProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AVyraProjectileBase>(ProjectileClass, FTransform(ProjectileDirection.Rotation(), SpawnLocation), PlayerStateCharacter))
			{
				Projectile->SetOwner(GetAvatarActorFromActorInfo());
				Projectile->DamageEffect = SpecHandle;

				UGameplayStatics::FinishSpawningActor(Projectile, FTransform(ProjectileDirection.Rotation(), SpawnLocation));
			}
		}
	}
}

int32 UArcadeAbility_EmberBurst::GetProjectileCount() const
{
	const int32 AbilityProjectiles = GetGameplayTagStackCount(FGameplayTag::RequestGameplayTag("GameplayTagStack.Arcade.Active.EmberBurst.ProjectileCount"));
	const int32 GlobalProjectileCount = GetGameplayTagStackCount(FGameplayTag::RequestGameplayTag("GameplayTagStack.Arcade.Global.ProjectileCount"));

	return AbilityProjectiles + GlobalProjectileCount;
}
