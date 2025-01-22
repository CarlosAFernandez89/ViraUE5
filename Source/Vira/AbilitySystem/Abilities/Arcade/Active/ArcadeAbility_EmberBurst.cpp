// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAbility_EmberBurst.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
				float ProjSpeedMulti = GetGameplayTagStackCount(FGameplayTag::RequestGameplayTag("GameplayTagStack.Arcade.Global.ProjectileSpeedMultiplier"));
				Projectile->UpdateProjectileSpeed(ProjSpeedMulti);

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

void UArcadeAbility_EmberBurst::OnCastingSucceeded_Implementation()
{
	Super::OnCastingSucceeded_Implementation();

	const int32 ProjectileCount = GetProjectileCount();
	const float ConeHalfAngleDegrees = UKismetMathLibrary::MapRangeClamped(ProjectileCount, 1.f,20.f,5.f, 45.f);
	SpawnProjectilesInCone(ProjectileCount, ConeHalfAngleDegrees);
}
