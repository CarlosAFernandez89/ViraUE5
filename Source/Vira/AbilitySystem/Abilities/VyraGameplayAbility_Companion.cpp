// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility_Companion.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Vira/AbilitySystem/Components/Companions/VyraCompanionPawn.h"

void UVyraGameplayAbility_Companion::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	/*
	 * Only very specific companions should get spawned when ability is given,
	 * such as passive companions and cosmetic companions.
	 */
	if (bSpawnOnGiveAbility)
	{
		SpawnCompanionPawn();
	}
}

void UVyraGameplayAbility_Companion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UVyraGameplayAbility_Companion::SpawnCompanionPawn()
{
	if (UWorld* World = GetWorld())
	{
		if (UNavigationSystemV1* NavigationSystemV1 = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
		{
			UE_LOG(LogTemp, Warning, TEXT("Navigation system valid."));

			FNavLocation Location = FNavLocation();

			if(!NavigationSystemV1->GetRandomReachablePointInRadius(GetAvatarActorFromActorInfo()->GetActorLocation(), SpawnRadius, Location))
			{
				UE_LOG(LogTemp, Error, TEXT("No random reachable location found."));
				return;
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			Location.Location += FVector(0.f,0.f, 90.f);
			if (AVyraCompanionPawn* CompanionPawn = World->SpawnActor<AVyraCompanionPawn>(CompanionPawnClass, Location.Location, FRotator(0, 0, 0), SpawnParams))
			{
				CompanionPawnReference = CompanionPawn;
				CompanionPawnReference->SetOwner(GetAvatarActorFromActorInfo());
				
				if (AAIController* AIController = CompanionPawnReference->GetController<AAIController>())
				{
					if (AIController->RunBehaviorTree(CompanionPawnBehavior))
					{
						if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
						{
							BBComponent->SetValueAsObject(FName("OwningActor"), GetAvatarActorFromActorInfo());
						}
					}
				}
			}
		}
	}
}

void UVyraGameplayAbility_Companion::DestroyCompanionPawn()
{
	if (CompanionPawnReference)
	{
		CompanionPawnReference->Destroy();
		CompanionPawnReference = nullptr;
	}
}
