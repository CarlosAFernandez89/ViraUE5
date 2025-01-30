// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "CombatAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatSetMovementSpeedChanged);
/**
 * 
 */
UCLASS()
class VIRA_API UCombatAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	UCombatAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//Offensive

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_BaseDamage)
	FGameplayAttributeData BaseDamage = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, BaseDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CastSpeed)
	FGameplayAttributeData CastSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CastSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CriticalStrikeChance)
	FGameplayAttributeData CriticalStrikeChance = 0.1f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CriticalStrikeChance);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CriticalStrikeDamageMultiplier)
	FGameplayAttributeData CriticalStrikeDamageMultiplier = 1.5f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, CriticalStrikeDamageMultiplier);
	
	//Defensive

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_DamageReduction)
	FGameplayAttributeData DamageReduction = 0.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, DamageReduction);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_KnockBackResistance)
	FGameplayAttributeData KnockBackResistance = 0.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, KnockBackResistance);

	// Utility

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintAssignable, Category = "CombatAttributeSet")
	FOnCombatSetMovementSpeedChanged OnCombatSetMovementSpeedChanged;

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_GlobalCooldownReduction)
	FGameplayAttributeData GlobalCooldownReduction = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, GlobalCooldownReduction);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_AirJumpCount)
	FGameplayAttributeData AirJumpCount = 1.f;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, AirJumpCount);

protected:
	UFUNCTION()
	virtual void OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage);
	UFUNCTION()
	virtual void OnRep_CastSpeed(const FGameplayAttributeData& OldAttackSpeed);
	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	UFUNCTION()
	virtual void OnRep_CriticalStrikeChance(const FGameplayAttributeData& OldCriticalStrikeChance);
	UFUNCTION()
	virtual void OnRep_CriticalStrikeDamageMultiplier(const FGameplayAttributeData& OldCriticalStrikeDamageMultiplier);

	UFUNCTION()
	virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction);
	UFUNCTION()
	virtual void OnRep_KnockBackResistance(const FGameplayAttributeData& OldKnockBackResistance);

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
	UFUNCTION()
	virtual void OnRep_AirJumpCount(const FGameplayAttributeData& OldAirJumpCount);
	UFUNCTION()
	virtual void OnRep_GlobalCooldownReduction(const FGameplayAttributeData& OldGlobalCooldownReduction);
};
