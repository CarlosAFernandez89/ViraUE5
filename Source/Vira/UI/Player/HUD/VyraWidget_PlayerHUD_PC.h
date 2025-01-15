// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/UI/Player/VyraCommonActivatableWidget_WithASC.h"
#include "VyraWidget_PlayerHUD_PC.generated.h"


USTRUCT(BlueprintType)
struct FVyraFailureMessage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FailureMessage;
	
};

class UGSCCoreComponent;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraWidget_PlayerHUD_PC : public UVyraCommonActivatableWidget_WithASC
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~ End UUserWidget interface

public:
	
	/** Init widget with attributes from owner character **/
	UFUNCTION(BlueprintCallable, Category="GAS Companion|UI")
	virtual void InitFromCharacter();

	/** Updates bound health widgets to reflect the new max health change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetMaxHealth(float MaxHealth);

	/** Updates bound health widgets to reflect the new health change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetHealth(float Health);

	/** Updates bound stamina progress bar with the new percent */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetHealthPercentage(float HealthPercentage);

	/** Updates bound stamina widgets to reflect the new max stamina change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetMaxStamina(float MaxStamina);

	/** Updates bound stamina widgets to reflect the new stamina change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetStamina(float Stamina);

	/** Updates bound health progress bar with the new percent */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetStaminaPercentage(float StaminaPercentage);

	/** Updates bound mana widgets to reflect the new max mana change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetMaxMana(float MaxMana);

	/** Updates bound mana widgets to reflect the new mana change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetMana(float Mana);

	/** Updates bound mana progress bar with the new percent */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetManaPercentage(float ManaPercentage);
	
	/** Updates bound mana widgets to reflect the new souls change */
	UFUNCTION(BlueprintCallable, Category = "GAS Companion|UI")
	virtual void SetSouls(float Souls);

protected:
	
	/** Set in native construct if called too early to check for ASC in tick, and kick off initialization logic when it is ready */
	bool bLazyAbilitySystemInitialization = false;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UTextBlock> StaminaText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UProgressBar> StaminaProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UTextBlock> ManaText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UProgressBar> ManaProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "GAS Companion|UI")
	TObjectPtr<UTextBlock> SoulsText;

	/** Updates bound widget whenever one of the attribute we care about is changed */
	virtual void HandleAttributeChange(FGameplayAttribute Attribute, float NewValue, float OldValue) override;

private:
	/** Array of active GE handle bound to delegates that will be fired when the count for the key tag changes to or away from zero */
	TArray<FActiveGameplayEffectHandle> GameplayEffectAddedHandles;

	/** Array of tags bound to delegates that will be fired when the count for the key tag changes to or away from zero */
	TArray<FGameplayTag> GameplayTagBoundToDelegates;

	static FString GetAttributeFormatString(float BaseValue, float MaxValue);

	/**
	 * Checks owner for a valid ASC and kick in initialization logic if it finds one
	 *
	 * @return Whether an ASC was found
	 */
	bool TryInitAbilitySystem();
};
