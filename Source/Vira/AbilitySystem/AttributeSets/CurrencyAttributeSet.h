// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSet.h"
#include "CurrencyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UCurrencyAttributeSet : public UGSCAttributeSet
{
	GENERATED_BODY()

public:

	UCurrencyAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	UPROPERTY(BlueprintReadOnly, Category = "CurrencyAttributeSet", ReplicatedUsing = OnRep_Souls)
	FGameplayAttributeData Souls = 1.f;
	ATTRIBUTE_ACCESSORS(UCurrencyAttributeSet, Souls);


	UPROPERTY(BlueprintReadOnly, Category = "CurrencyAttributeSet", ReplicatedUsing = OnRep_PickUpRadius)
	FGameplayAttributeData PickUpRadius = 1.f;
	ATTRIBUTE_ACCESSORS(UCurrencyAttributeSet, PickUpRadius);

	UPROPERTY(BlueprintReadOnly, Category = "CurrencyAttributeSet", ReplicatedUsing = OnRep_ItemDropRateIncrease)
	FGameplayAttributeData ItemDropRateIncrease = 1.f;
	ATTRIBUTE_ACCESSORS(UCurrencyAttributeSet, ItemDropRateIncrease);

protected:
	UFUNCTION()
	virtual void OnRep_Souls(const FGameplayAttributeData& OldSouls);

	UFUNCTION()
	virtual void OnRep_PickUpRadius(const FGameplayAttributeData& OldPickUpRadius);

	UFUNCTION()
	virtual void OnRep_ItemDropRateIncrease(const FGameplayAttributeData& OldItemDropRateIncrease);
};
