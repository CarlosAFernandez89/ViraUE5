// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_AbilityGameplayTagStack.h"
#include "Abilities/GSCGameplayAbility.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "VyraGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FVyraGameplayAbilityInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	FText Name = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	FText Description = FText();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	UTexture2D* Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	UCurveTable* DamageCurveTable = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityLevelChanged, int32, NewLevel);

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
	FORCEINLINE class AVyraPlayerStateCharacter* GetVyraPlayerStateCharacter() const
	{
		return PlayerStateCharacter;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UAnimInstance* GetVyraAnimInstance() const
	{
		return PlayerStateCharacter ? PlayerStateCharacter->GetMesh()->GetAnimInstance() : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UVyraAbilitySystemComponent* GetVyraAbilitySystemComponent() const
	{
		return PlayerStateCharacter ? Cast<UVyraAbilitySystemComponent>(PlayerStateCharacter->GetAbilitySystemComponent()) : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UGSCCoreComponent* GetVyraGSCCoreComponent() const
	{
		return PlayerStateCharacter ? PlayerStateCharacter->GetGSCCoreComponent() : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVyraGameplayAbilityInfo GetAbilityInfo() const { return AbilityInfo; }

	UFUNCTION(BlueprintCallable)
	void UpdateAbilityLevel(const int32 NewAbilityLevel) const
	{
		GetCurrentAbilitySpec()->Level = NewAbilityLevel;
	}

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetAbilityLevelTag() const
	{
		return AbilityGameplayTagStack ? AbilityGameplayTagStack->GetAbilityLevelTagStackData().GameplayTag : FGameplayTag();
	}
	
protected:

	UFUNCTION(BlueprintNativeEvent)
	void OnGameplayTagStackUpdated(FGameplayTag UpdatedTag, const int32 NewTagCount);

	UFUNCTION(BlueprintCallable)
	float GetGameplayTagStackCount(const FGameplayTag GameplayTag) const
	{
		return GetVyraAbilitySystemComponent()->GetGameplayTagStackCount(GameplayTag);
	}

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual float GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const override;
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayEffect> GetDamageGameplayEffectClass() const
	{
		return AbilityInfo.DamageEffect;
	};
	
	UFUNCTION(BlueprintCallable)
	bool GetMouseLocation(FVector& HitLocation, float TraceDistance) const;

	UFUNCTION(BlueprintCallable, Category = "VyraGameplayAbility|Arcade")
	float GetCastSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "VyraGameplayAbility|Arcade")
	float GetAttackSpeed() const;
protected:

	UPROPERTY()
	class AVyraPlayerStateCharacter* PlayerStateCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vyra|Ability")
	FVyraGameplayAbilityInfo AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vyra|Ability|GameplayTags")
	UDataAsset_AbilityGameplayTagStack* AbilityGameplayTagStack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Vyra|Ability|Default")
	UAnimMontage* MontageToPlay;
	
	UPROPERTY()
	class UCommonInputSubsystem* CommonInputSubsystem;
	
	UPROPERTY()
	class APlayerController* PlayerController;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityLevelChanged OnAbilityLevelChanged;
	
};
