// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraCheatManager.h"

#include "VyraPlayerController.h"
#include "VyraPlayerState.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"

UVyraCheatManager::UVyraCheatManager()
{
}

void UVyraCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
}

void UVyraCheatManager::DamageSelf(const float DamageAmount)
{
	if(UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
	{
		ApplySetByCallerDamage(ASC, DamageAmount);
	}
}

void UVyraCheatManager::ApplySetByCallerDamage(class UVyraAbilitySystemComponent* ASC, float DamageAmount) const
{
	check(ASC);

	if(DamageGE)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageGE, 1.f, ASC->MakeEffectContext());
		if(SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Damage"), DamageAmount);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

UVyraAbilitySystemComponent* UVyraCheatManager::GetVyraAbilitySystemComponent() const
{
	if(const AVyraPlayerController* PC = Cast<AVyraPlayerController>(GetOuterAPlayerController()))
	{
		return Cast<UVyraAbilitySystemComponent>(PC->GetPlayerState<AVyraPlayerState>()->GetAbilitySystemComponent());
	}
	return nullptr;
}
