// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCGameplayAbility.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "VyraGameplayAbility.generated.h"


USTRUCT(BlueprintType)
struct FVyraGameplayAbilityInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "None";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
};

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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVyraGameplayAbilityInfo GetAbilityInfo() const { return AbilityInfo; }
	
protected:
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:

	UPROPERTY()
	class AVyraPlayerStateCharacter* PlayerStateCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vyra|Ability")
	FVyraGameplayAbilityInfo AbilityInfo;

};
