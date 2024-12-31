// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraPlayerState.h"

#include "GSCLog.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Misc/EngineVersionComparison.h"

AVyraPlayerState::AVyraPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UVyraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// By default, specifically set abilities to persist across deaths / respawns or possessions
	//
	// Note: This is to avoid an issue with ability input binding with Player State setups, as re-granting the ability
	// and trying to bind input again is giving trouble (whereas keeping them around solves this).
	// ASC on Pawns doesn't have this problem.
	AbilitySystemComponent->bResetAbilitiesOnSpawn = false;
	
	// Replication Mode is set in PostInitProperties to allow users to change the default value from BP

	// Set PlayerState's NetUpdateFrequency to sensible defaults.
	//
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
#if UE_VERSION_OLDER_THAN(5, 5, 0)
	NetUpdateFrequency = 10.f;
#else
	SetNetUpdateFrequency(10.f);
#endif
}

void AVyraPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AVyraPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void AVyraPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void AVyraPlayerState::PostInitProperties()
{
	Super::PostInitProperties();
	if (AbilitySystemComponent)
	{
		GSC_LOG(Verbose, TEXT("AGSCModularPlayerState::PostInitProperties for %s - Setting up ASC Replication Mode to: %d"), *GetName(), ReplicationMode)
		AbilitySystemComponent->SetReplicationMode(ReplicationMode);
	}
}

void AVyraPlayerState::Reset()
{
	Super::Reset();

	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UPlayerStateComponent* Component : ModularComponents)
	{
		Component->Reset();
	}
}

UAbilitySystemComponent* AVyraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AVyraPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	TInlineComponentArray<UPlayerStateComponent*> PlayerStateComponents;
	GetComponents(PlayerStateComponents);
	for (UPlayerStateComponent* SourceComponent : PlayerStateComponents)
	{
		if (UPlayerStateComponent* TargetComp = Cast<UPlayerStateComponent>(static_cast<UObject*>(FindObjectWithOuter(PlayerState, SourceComponent->GetClass(), SourceComponent->GetFName()))))
		{
			SourceComponent->CopyProperties(TargetComp);
		}
	}
}
