// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraCompanionPawn_WithASC.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/AbilitySystem/AttributeSets/Companions/CompanionAttributeSetBase.h"


// Sets default values
AVyraCompanionPawn_WithASC::AVyraCompanionPawn_WithASC()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UVyraAbilitySystemComponent>("AbilitySystemComponent");
	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>("GSCCoreComponent");
}

void AVyraCompanionPawn_WithASC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent)
	{
		bool bFoundMovementSpeed = false;
		const float MovementSpeed = UAbilitySystemBlueprintLibrary::GetFloatAttributeFromAbilitySystemComponent(AbilitySystemComponent, UCompanionAttributeSetBase::GetMovementSpeedAttribute(), bFoundMovementSpeed);

		if (bFoundMovementSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
		}
	}
}

