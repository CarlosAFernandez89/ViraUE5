// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "NativeGameplayTags.h"
#include "VyraGhostProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_PROJECTILES_FORK, "GameplayTagStack.Arcade.Global.ProjectilesFork");
UE_DEFINE_GAMEPLAY_TAG(TAG_PROJECTILES_FORK_COUNT, "GameplayTagStack.Arcade.Global.ProjectilesFork.Count");

// Sets default values
AVyraProjectileBase::AVyraProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(32.f);
	SphereComponent->SetEnableGravity(false);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AVyraProjectileBase::SphereComponentBeginOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = 850.f;
	ProjectileMovementComponent->MaxSpeed = 850.f;
	ProjectileMovementComponent->Velocity = FVector(1.f, 0.f, 0.f);
}

void AVyraProjectileBase::SetProjectileSpeed(float InProjectileSpeed) const
{
	ProjectileMovementComponent->MaxSpeed = InProjectileSpeed;
	ProjectileMovementComponent->InitialSpeed = InProjectileSpeed;
}

void AVyraProjectileBase::UpdateProjectileSpeed(const float InSpeedMultiplier) const
{
	ProjectileMovementComponent->MaxSpeed *= InSpeedMultiplier;
	ProjectileMovementComponent->InitialSpeed = ProjectileMovementComponent->MaxSpeed;
}

void AVyraProjectileBase::SetupProjectileOwnerValues(ACharacter* InOwningCharacter,
                                                     UVyraAbilitySystemComponent* InOwningASC, const bool InShouldFork)
{
	if (InOwningCharacter)
	{
		OwningCharacter = InOwningCharacter;
	}

	if (InOwningASC)
	{
		OwnerASC = InOwningASC;
	}

	bAlreadyForked = InShouldFork;
}

void AVyraProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//We can set these when spawning the projectile to avoid getting the references.
	// Should probably get done for Sub-projectiles such as fork/chain/split.
	if (!OwningCharacter || !OwnerASC)
	{
		if (AActor* OwnerActor = GetOwner())
		{
			if (ACharacter* OwnerPawn = Cast<ACharacter>(OwnerActor))
			{
				OwningCharacter = OwnerPawn;

				if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningCharacter))
				{
					OwnerASC = CastChecked<UVyraAbilitySystemComponent>(ASC);
				}
			}
		}
	}

	UpdateProjectileSpeed();
	SpawnVisuals();
	SetLifeSpan(CalculateLifeSpanLinear());
}

void AVyraProjectileBase::SpawnVisuals()
{
	if (AVyraGhostProjectile* GhostProjectile = GetWorld()->SpawnActorDeferred<AVyraGhostProjectile>(AVyraGhostProjectile::StaticClass(), GetActorTransform(), this))
	{
		GhostProjectile->InitializeNiagaraSystem(ProjectileNiagaraSystem);

		UGameplayStatics::FinishSpawningActor(GhostProjectile, GetActorTransform());
	}
}

float AVyraProjectileBase::CalculateLifeSpanLinear() const
{
	constexpr float ReferenceSpeed = 850.0f;
	constexpr float ReferenceLifespan = 1.5f;
	constexpr float TargetDistance = ReferenceSpeed * ReferenceLifespan;

	if (ProjectileMovementComponent->GetMaxSpeed() <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile speed must be positive."));
		return 0.0f; // Or some default/minimum lifespan
	}

	return TargetDistance / ProjectileMovementComponent->GetMaxSpeed();
}

bool AVyraProjectileBase::ShouldProjectileFork() const
{
	if (OwnerASC)
	{
		const float TagCount = OwnerASC->GetGameplayTagStackCount(TAG_PROJECTILES_FORK);
		const int32 IntTagCount = FMath::FloorToInt(TagCount);
		return (IntTagCount > 0) && !bAlreadyForked;
	}
	return false;
}

void AVyraProjectileBase::ForkProjectile(AActor* ForkInstigator) const
{
	if (OwnerASC)
	{
		const float ForkCountFloat = OwnerASC->GetGameplayTagStackCount(TAG_PROJECTILES_FORK_COUNT);
		const int32 ForkCount = FMath::FloorToInt(ForkCountFloat);

		if (ForkCount >= 1)
		{
			const FVector CurrentDirection = GetActorForwardVector();
			const FVector CurrentLocation = GetActorLocation();

			// Define the maximum random angle deviation (in degrees)
			const float MaxRandomAngleDeviation = 180.0f;

			for (int32 i = 0; i < ForkCount; i++)
			{
				float RandomAngle = FMath::FRandRange(-MaxRandomAngleDeviation, MaxRandomAngleDeviation);
				FVector ForkedProjectileDirection = CurrentDirection.RotateAngleAxis(RandomAngle, FVector::UpVector);
				
				TSubclassOf<AVyraProjectileBase> ThisProjectileClass = GetClass();
				FTransform ProjectileTransform = FTransform(ForkedProjectileDirection.Rotation(), CurrentLocation);
				
				if (AVyraProjectileBase* Projectile = GetWorld()->SpawnActorDeferred<AVyraProjectileBase>(ThisProjectileClass, ProjectileTransform, GetOwner()))
				{
					Projectile->DamageEffect = DamageEffect;
					Projectile->ProjectileInstigator = ForkInstigator;
					Projectile->SetupProjectileOwnerValues(OwningCharacter, OwnerASC, true);
					Projectile->SetProjectileSpeed(ProjectileMovementComponent->MaxSpeed);
					
					// Finish spawning the projectile
					UGameplayStatics::FinishSpawningActor(Projectile, ProjectileTransform);
				}
			}
		}
	}
}

bool AVyraProjectileBase::IsProjectilePhasing() const
{
	if (OwnerASC && ProjectilePhasingTag.IsValid())
	{
		const float PhasingCount =OwnerASC->GetGameplayTagStackCount(ProjectilePhasingTag);
		const int32 IntPhasingCount = FMath::FloorToInt(PhasingCount);
		return (IntPhasingCount > 0);
	}
	return false;
}

void AVyraProjectileBase::OnHitActor_Implementation(AActor* HitActor, const FHitResult& SweepResult)
{
	if (ShouldProjectileFork())
	{
		ForkProjectile(HitActor);
	}


	// Check if projectile should be destroyed after all other interactions have triggered.
	if (!IsProjectilePhasing())
	{
		Destroy();
	}
}

void AVyraProjectileBase::OnHitWorldObject_Implementation(const FHitResult& SweepResult)
{
	Destroy();
}

void AVyraProjectileBase::SphereComponentBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent,
                                                                     AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                     const FHitResult& SweepResult)
{
	// Make sure the same actor doesn't proc projectile effects when it spawns projectile effects.
	if (OtherActor == ProjectileInstigator) return;
	
	if (!OtherActor->IsA(TargetClass))
	{
		if (!OtherActor->IsA(AVyraPlayerStateCharacter::StaticClass()))
		{
			OnHitWorldObject(SweepResult);
		}
		return;
	}

	if (!DamageEffect.IsValid()) return;
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor, true))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffect.Data.Get());
		OnHitActor(OtherActor, SweepResult);
	}
}

