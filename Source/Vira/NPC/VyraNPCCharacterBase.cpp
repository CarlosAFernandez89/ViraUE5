// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraNPCCharacterBase.h"

#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"


AVyraNPCCharacterBase::AVyraNPCCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>("GSCCoreComponent");
	VyraAbilitySystemComponent = CreateDefaultSubobject<UVyraAbilitySystemComponent>("VyraAbilitySystemComponent");

	bUseControllerRotationYaw = false;
	if(UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bOrientRotationToMovement = true;
		CharacterMovementComponent->RotationRate = FRotator(0.0f, 650.f, 0.0f);
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
}

UAbilitySystemComponent* AVyraNPCCharacterBase::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(VyraAbilitySystemComponent);
}
