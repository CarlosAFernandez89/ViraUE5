// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "CompanionAttributeSetBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompanionMovementSpeedChanged);
/**
 * 
 */
UCLASS()
class VIRA_API UCompanionAttributeSetBase : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	UCompanionAttributeSetBase();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//Offensive

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_BaseDamage)
	FGameplayAttributeData BaseDamage = 1.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, BaseDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CastSpeed)
	FGameplayAttributeData CastSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, CastSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CriticalStrikeChance)
	FGameplayAttributeData CriticalStrikeChance = 0.1f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, CriticalStrikeChance);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_CriticalStrikeDamageMultiplier)
	FGameplayAttributeData CriticalStrikeDamageMultiplier = 1.5f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, CriticalStrikeDamageMultiplier);

	//Defensive

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_DamageReduction)
	FGameplayAttributeData DamageReduction = 0.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, DamageReduction);

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_KnockBackResistance)
	FGameplayAttributeData KnockBackResistance = 0.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, KnockBackResistance);

	// Utility

	UPROPERTY(BlueprintReadOnly, Category = "CombatAttributeSet", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed = 1.f;
	ATTRIBUTE_ACCESSORS(UCompanionAttributeSetBase, MovementSpeed);

	UPROPERTY(BlueprintAssignable, Category = "CombatAttributeSet")
	FOnCompanionMovementSpeedChanged OnCompanionMovementSpeedChanged;

protected:
	UFUNCTION()
	virtual void OnRep_BaseDamage(const FGameplayAttributeData& OldBaseDamage);
	UFUNCTION()
	virtual void OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed);
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
};
