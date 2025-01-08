// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_LootPickup.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Vira/AbilitySystem/AttributeSets/CurrencyAttributeSet.h"
#include "Vira/Items/Currency/CurrencyDropBase.h"

UGameplayAbility_LootPickup::UGameplayAbility_LootPickup()
{
}

void UGameplayAbility_LootPickup::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	GetWorld()->GetTimerManager().SetTimer(PullTimerHandle, this, &UGameplayAbility_LootPickup::PullCurrencyLoop, 1.f, true);
}

void UGameplayAbility_LootPickup::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_LootPickup::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_LootPickup::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, PullTimerHandle);
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UGameplayAbility_LootPickup::PullCurrencyLoop()
{
	float Radius = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UCurrencyAttributeSet::GetPickUpRadiusAttribute());

	for (TSubclassOf<ACurrencyDropBase> Currency : CurrencyToPickUp)
	{
		PullActorsToPlayer(Currency, Radius);
	}
}
void UGameplayAbility_LootPickup::PullActorsToPlayer(TSubclassOf<ACurrencyDropBase> CurrencyClass, float InRadius)
{
	if (!CurrencyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PullActorsToPlayer: Invalid CurrencyClass!"));
		return;
	}
	
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("PullActorsToPlayer: Invalid AvatarActor!"));
		return;
	}

	const FVector AvatarLocation = AvatarActor->GetActorLocation();
	
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),                 // World context
		AvatarLocation,             // Sphere center
		InRadius,                   // Sphere radius
		TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) }, // Object types to overlap, adjust as needed. Here it is looking for WorldDynamic.
		CurrencyClass,              // Actor class filter, in this case, we only filter by this class.
		ActorsToIgnore,				// Ignore self
		OverlappingActors       // Output array
	);

	if (bDebug)
	{
		// Print the number of overlapping actors to the screen
		int32 NumOverlappingActors = OverlappingActors.Num();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Found %d overlapping actors of class %s"), NumOverlappingActors, *CurrencyClass->GetName()));
	}
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (ACurrencyDropBase* CurrencyActor = Cast<ACurrencyDropBase>(OverlappingActor))
		{
			CurrencyActor->StartLerpToPlayer(AvatarActor, LerpSpeed);
		}
	}
}
