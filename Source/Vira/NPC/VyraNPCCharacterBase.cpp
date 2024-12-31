// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraNPCCharacterBase.h"

#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"


AVyraNPCCharacterBase::AVyraNPCCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(45.f);

	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>("GSCCoreComponent");

	PaperZDAnimationComponent = CreateDefaultSubobject<UPaperZDAnimationComponent>("PaperZDAnimationComponent");

	// Try to create the sprite component
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(APaperCharacter::SpriteComponentName);
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Sprite->SetupAttachment(GetCapsuleComponent());
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);
		Sprite->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

	bUseControllerRotationYaw = false;
	if(UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bOrientRotationToMovement = true;
		CharacterMovementComponent->RotationRate = FRotator(0.0f, -1.f, 0.0f);
		CharacterMovementComponent->bConstrainToPlane = true;
		CharacterMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
	}
}

void AVyraNPCCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(this))
	{
		if (Sprite)
		{
			// force animation tick after movement component updates
			if (Sprite->PrimaryComponentTick.bCanEverTick && GetCharacterMovement())
			{
				Sprite->PrimaryComponentTick.AddPrerequisite(GetCharacterMovement(), GetCharacterMovement()->PrimaryComponentTick);
			}
		}
	}
}

// Called when the game starts or when spawned
void AVyraNPCCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

