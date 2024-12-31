// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraWidget_PlayerHUD_PC.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GSCLog.h"
#include "Abilities/Attributes/GSCAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "Vira/AbilitySystem/AttributeSets/CurrencyAttributeSet.h"

void UVyraWidget_PlayerHUD_PC::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPlayerPawn = GetOwningPlayerPawn())
	{
		SetOwnerActor(OwningPlayerPawn);

		if (!TryInitAbilitySystem())
		{
			GSC_LOG(Warning, TEXT("UVyraWidget_PlayerHUD_PC::NativeConstruct called too early: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwningPlayerPawn))
			bLazyAbilitySystemInitialization = true;
		}
	}
}

void UVyraWidget_PlayerHUD_PC::NativeDestruct()
{
	// Clean up previously registered delegates for OwningPlayer AbilitySystemComponent
	ResetAbilitySystem();

	Super::NativeDestruct();
}

void UVyraWidget_PlayerHUD_PC::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Lazy init disabled, was either good from native construct or initialized once here
	if (!bLazyAbilitySystemInitialization)
	{
		return;
	}

	if (TryInitAbilitySystem())
	{
		// We now have proper ASC and initialized widget, prevent further tick run
		bLazyAbilitySystemInitialization = false;
		
		GSC_LOG(Warning, TEXT("UVyraWidget_PlayerHUD_PC::NativeTick reconciliated with ASC. We now have a reference to it: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))
	}
}

void UVyraWidget_PlayerHUD_PC::InitFromCharacter()
{
	if (!AbilitySystemComponent)
	{
		// Prevent calls to GetAttributeValue if no ASC yet, which may happen for clients (and prevent warning logs during initialization)
		return;
	}

	SetHealth(GetAttributeValue(UGSCAttributeSet::GetHealthAttribute()));
	SetStamina(GetAttributeValue(UGSCAttributeSet::GetStaminaAttribute()));
	SetMana(GetAttributeValue(UGSCAttributeSet::GetManaAttribute()));
	SetSouls(GetAttributeValue(UCurrencyAttributeSet::GetSoulsAttribute()));
}

void UVyraWidget_PlayerHUD_PC::SetMaxHealth(const float MaxHealth)
{
	const float Health = GetAttributeValue(UGSCAttributeSet::GetHealthAttribute());
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(GetAttributeFormatString(Health, MaxHealth)));
	}

	if (MaxHealth != 0)
	{
		SetHealthPercentage(Health / MaxHealth);
	}
}

void UVyraWidget_PlayerHUD_PC::SetHealth(const float Health)
{
	const float MaxHealth = GetAttributeValue(UGSCAttributeSet::GetMaxHealthAttribute());
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(GetAttributeFormatString(Health, MaxHealth)));
	}

	if (MaxHealth != 0)
	{
		SetHealthPercentage(Health / MaxHealth);
	}
}

void UVyraWidget_PlayerHUD_PC::SetHealthPercentage(const float HealthPercentage)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercentage);
	}
}

void UVyraWidget_PlayerHUD_PC::SetMaxStamina(const float MaxStamina)
{
	const float Stamina = GetAttributeValue(UGSCAttributeSet::GetStaminaAttribute());
	if (StaminaText)
	{
		StaminaText->SetText(FText::FromString(GetAttributeFormatString(Stamina, MaxStamina)));
	}

	if (MaxStamina != 0)
	{
		SetStaminaPercentage(Stamina / MaxStamina);
	}
}

void UVyraWidget_PlayerHUD_PC::SetStamina(const float Stamina)
{
	const float MaxStamina = GetAttributeValue(UGSCAttributeSet::GetMaxStaminaAttribute());
	if (StaminaText)
	{
		StaminaText->SetText(FText::FromString(GetAttributeFormatString(Stamina, MaxStamina)));
	}

	if (MaxStamina != 0)
	{
		SetStaminaPercentage(Stamina / MaxStamina);
	}
}

void UVyraWidget_PlayerHUD_PC::SetStaminaPercentage(const float StaminaPercentage)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(StaminaPercentage);
	}
}

void UVyraWidget_PlayerHUD_PC::SetMaxMana(const float MaxMana)
{
	const float Mana = GetAttributeValue(UGSCAttributeSet::GetManaAttribute());
	if (ManaText)
	{
		ManaText->SetText(FText::FromString(GetAttributeFormatString(Mana, MaxMana)));
	}

	if (MaxMana != 0)
	{
		SetManaPercentage(Mana / MaxMana);
	}
}

void UVyraWidget_PlayerHUD_PC::SetMana(const float Mana)
{
	const float MaxMana = GetAttributeValue(UGSCAttributeSet::GetMaxManaAttribute());
	if (ManaText)
	{
		ManaText->SetText(FText::FromString(GetAttributeFormatString(Mana, MaxMana)));
	}

	if (MaxMana != 0)
	{
		SetManaPercentage(Mana / MaxMana);
	}
}

void UVyraWidget_PlayerHUD_PC::SetManaPercentage(const float ManaPercentage)
{
	if (ManaProgressBar)
	{
		ManaProgressBar->SetPercent(ManaPercentage);
	}
}

void UVyraWidget_PlayerHUD_PC::SetSouls(float Souls)
{
	if(SoulsText)
	{
		// Format the text with the Souls value
		FText FormattedText = FText::Format(
			NSLOCTEXT("YourNamespace", "SoulsDisplay", "Souls: {0}"), // The format string
			FText::AsNumber(Souls) // Replace {0} with the Souls value
		);
		SoulsText->SetText(FormattedText);
	}
}

void UVyraWidget_PlayerHUD_PC::HandleAttributeChange(const FGameplayAttribute Attribute, const float NewValue, const float OldValue)
{
	if (Attribute == UGSCAttributeSet::GetHealthAttribute())
	{
		SetHealth(NewValue);
	}
	else if (Attribute == UGSCAttributeSet::GetStaminaAttribute())
	{
		SetStamina(NewValue);
	}
	else if (Attribute == UGSCAttributeSet::GetManaAttribute())
	{
		SetMana(NewValue);
	}
	else if (Attribute == UGSCAttributeSet::GetMaxHealthAttribute())
	{
		SetMaxHealth(NewValue);
	}
	else if (Attribute == UGSCAttributeSet::GetMaxStaminaAttribute())
	{
		SetMaxStamina(NewValue);
	}
	else if (Attribute == UGSCAttributeSet::GetMaxManaAttribute())
	{
		SetMaxMana(NewValue);
	}
	else if (Attribute == UCurrencyAttributeSet::GetSoulsAttribute())
	{
		SetSouls(NewValue);
	}
}

FString UVyraWidget_PlayerHUD_PC::GetAttributeFormatString(const float BaseValue, const float MaxValue)
{
	return FString::Printf(TEXT("%d / %d"), FMath::FloorToInt(BaseValue), FMath::FloorToInt(MaxValue));
}

bool UVyraWidget_PlayerHUD_PC::TryInitAbilitySystem()
{
	GSC_LOG(Verbose, TEXT("UVyraWidget_PlayerHUD_PC::TryInitAbilitySystem check for ASC: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))

	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
	}

	if (AbilitySystemComponent)
	{
		bLazyAbilitySystemInitialization = false;

		GSC_LOG(Verbose, TEXT("UVyraWidget_PlayerHUD_PC::TryInitAbilitySystem check found a new ASC: %s (%s)"), *GetNameSafe(AbilitySystemComponent), *GetNameSafe(OwnerActor))

		InitializeWithAbilitySystem(AbilitySystemComponent);

		// Init Stats
		InitFromCharacter();
	}

	return AbilitySystemComponent != nullptr;
}

