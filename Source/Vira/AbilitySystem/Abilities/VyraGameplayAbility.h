// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "VyraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VIRA_API UVyraGameplayAbility : public UGSCGameplayAbility
{
	GENERATED_BODY()

public:
	UVyraGameplayAbility();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AVyraPlayerStateCharacter* GetVyraPlayerStateCharacter() const { return PlayerStateCharacter; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAnimInstance* GetVyraAnimInstance() const { return PlayerStateCharacter->GetMesh()->GetAnimInstance(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UVyraAbilitySystemComponent* GetVyraAbilitySystemComponent() const { return Cast<UVyraAbilitySystemComponent>(PlayerStateCharacter->GetAbilitySystemComponent()); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UGSCCoreComponent* GetVyraGSCCoreComponent() const { return PlayerStateCharacter->GetGSCCoreComponent(); }
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	

protected:

	UPROPERTY()
	class AVyraPlayerStateCharacter* PlayerStateCharacter;

};
