// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraCommonActivatableWidget_WithASC.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GSCBlueprintFunctionLibrary.h"
#include "GSCLog.h"

void UVyraCommonActivatableWidget_WithASC::SetOwnerActor(AActor* Actor)
{
	OwnerActor = Actor;
	OwnerCoreComponent = UGSCBlueprintFunctionLibrary::GetCompanionCoreComponent(Actor);
	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
}

// ReSharper disable once CppParameterNamesMismatch
void UVyraCommonActivatableWidget_WithASC::InitializeWithAbilitySystem(const UAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		GSC_LOG(Error, TEXT("UVyraCommonActivatableWidget_WithASC::InitializeWithAbilitySystem - Called with invalid ASC"))
		return;
	}

	if (!OwnerActor)
	{
		SetOwnerActor(InASC->GetOwnerActor());
	}

	if (AbilitySystemComponent && AbilitySystemComponent != InASC)
	{
		ResetAbilitySystem();
	}

	AbilitySystemComponent = const_cast<UAbilitySystemComponent*>(InASC);
	RegisterAbilitySystemDelegates();

	// Broadcast info to Blueprints
	OnAbilitySystemInitialized();
}

void UVyraCommonActivatableWidget_WithASC::ResetAbilitySystem()
{
	ShutdownAbilitySystemComponentListeners();
	AbilitySystemComponent = nullptr;
}

void UVyraCommonActivatableWidget_WithASC::RegisterAbilitySystemDelegates()
{
	if (!AbilitySystemComponent)
	{
		// Ability System may not have been available yet for character (PlayerState setup on clients)
		return;
	}

	TArray<FGameplayAttribute> Attributes;
	AbilitySystemComponent->GetAllAttributes(Attributes);

	for (FGameplayAttribute Attribute : Attributes)
	{
		GSC_LOG(Verbose, TEXT("UVyraCommonActivatableWidget_WithASC::SetupAbilitySystemComponentListeners - Setup callback for %s (%s)"), *Attribute.GetName(), *GetNameSafe(OwnerActor));
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnAttributeChanged);
	}

	// Handle GameplayEffects added / remove
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectAdded);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnAnyGameplayEffectRemoved);

	// Handle generic GameplayTags added / removed
	AbilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnAnyGameplayTagChanged);

	// Handle Ability Commit events
	AbilitySystemComponent->AbilityCommittedCallbacks.AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnAbilityCommitted);
}

void UVyraCommonActivatableWidget_WithASC::ShutdownAbilitySystemComponentListeners() const
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	TArray<FGameplayAttribute> Attributes;
	AbilitySystemComponent->GetAllAttributes(Attributes);

	for (const FGameplayAttribute& Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	AbilitySystemComponent->RegisterGenericGameplayTagEvent().RemoveAll(this);
	AbilitySystemComponent->AbilityCommittedCallbacks.RemoveAll(this);

	for (const FActiveGameplayEffectHandle GameplayEffectAddedHandle : GameplayEffectAddedHandles)
	{
		if (GameplayEffectAddedHandle.IsValid())
		{
			FOnActiveGameplayEffectStackChange* EffectStackChangeDelegate = AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(GameplayEffectAddedHandle);
			if (EffectStackChangeDelegate)
			{
				EffectStackChangeDelegate->RemoveAll(this);
			}

			FOnActiveGameplayEffectTimeChange* EffectTimeChangeDelegate = AbilitySystemComponent->OnGameplayEffectTimeChangeDelegate(GameplayEffectAddedHandle);
			if (EffectTimeChangeDelegate)
			{
				EffectTimeChangeDelegate->RemoveAll(this);
			}
		}
	}

	for (const FGameplayTag GameplayTagBoundToDelegate : GameplayTagBoundToDelegates)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTagBoundToDelegate).RemoveAll(this);
	}
}

float UVyraCommonActivatableWidget_WithASC::GetPercentForAttributes(const FGameplayAttribute Attribute, const FGameplayAttribute MaxAttribute) const
{
	const float AttributeValue = GetAttributeValue(Attribute);
	const float MaxAttributeValue = GetAttributeValue(MaxAttribute);

	if (MaxAttributeValue == 0.f)
	{
		GSC_LOG(Warning, TEXT("UVyraCommonActivatableWidget_WithASC::GetPercentForAttributes() MaxAttribute %s value is 0. This leads to divide by Zero errors, will return 0"), *MaxAttribute.GetName())
		return 0.f;
	}

	return AttributeValue / MaxAttributeValue;
}

float UVyraCommonActivatableWidget_WithASC::GetAttributeValue(const FGameplayAttribute Attribute) const
{
	if (!AbilitySystemComponent)
	{
		GSC_WLOG(Error, TEXT("The owner AbilitySystemComponent seems to be invalid. Will return 0.f."))
		return 0.f;
	}

	if (!Attribute.IsValid())
	{
		GSC_WLOG(Error, TEXT("Passed in Attribute is invalid (None). Will return 0.f."))
		return 0.f;
	}

	if (!AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		const UClass* AttributeSet = Attribute.GetAttributeSetClass();
		GSC_WLOG(
			Error,
			TEXT("Trying to get value of attribute [%s.%s]. %s doesn't seem to be granted to %s. Returning 0.f"),
			*GetNameSafe(AttributeSet),
			*Attribute.GetName(),
			*GetNameSafe(AttributeSet),
			*GetNameSafe(AbilitySystemComponent)
		);
		
		return 0.f;
	}

	return AbilitySystemComponent->GetNumericAttribute(Attribute);
}

void UVyraCommonActivatableWidget_WithASC::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	// Broadcast event to Blueprint
	OnAttributeChange(Data.Attribute, Data.NewValue, Data.OldValue);

	// Trigger post attribute change event for subclass that needs further handling
	HandleAttributeChange(Data.Attribute, Data.NewValue, Data.OldValue);
}

void UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, const FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AbilitySystemComponent)
	{
		if (FOnActiveGameplayEffectStackChange* Delegate = AbilitySystemComponent->OnGameplayEffectStackChangeDelegate(ActiveHandle))
		{
			Delegate->AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectStackChanged);
		}

		if (FOnActiveGameplayEffectTimeChange* Delegate = AbilitySystemComponent->OnGameplayEffectTimeChangeDelegate(ActiveHandle))
		{
			Delegate->AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectTimeChanged);
		}

		// Store active handles to clear out bound delegates when shutting down listeners
		GameplayEffectAddedHandles.AddUnique(ActiveHandle);
	}

	HandleGameplayEffectAdded(AssetTags, GrantedTags, ActiveHandle);
}

void UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectStackChanged(const FActiveGameplayEffectHandle ActiveHandle, const int32 NewStackCount, const int32 PreviousStackCount)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FActiveGameplayEffect* GameplayEffect = AbilitySystemComponent->GetActiveGameplayEffect(ActiveHandle);
	if (!GameplayEffect)
	{
		return;
	}

	FGameplayTagContainer AssetTags;
	GameplayEffect->Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	GameplayEffect->Spec.GetAllGrantedTags(GrantedTags);

	HandleGameplayEffectStackChange(AssetTags, GrantedTags, ActiveHandle, NewStackCount, PreviousStackCount);
}

void UVyraCommonActivatableWidget_WithASC::OnActiveGameplayEffectTimeChanged(const FActiveGameplayEffectHandle ActiveHandle, const float NewStartTime, const float NewDuration)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FActiveGameplayEffect* GameplayEffect = AbilitySystemComponent->GetActiveGameplayEffect(ActiveHandle);
	if (!GameplayEffect)
	{
		return;
	}

	FGameplayTagContainer AssetTags;
	GameplayEffect->Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	GameplayEffect->Spec.GetAllGrantedTags(GrantedTags);

	HandleGameplayEffectTimeChange(AssetTags, GrantedTags, ActiveHandle, NewStartTime, NewDuration);
}

void UVyraCommonActivatableWidget_WithASC::OnAnyGameplayEffectRemoved(const FActiveGameplayEffect& EffectRemoved)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);

	// Broadcast any GameplayEffect change to HUD
	HandleGameplayEffectStackChange(AssetTags, GrantedTags, EffectRemoved.Handle, 0, 1);
	HandleGameplayEffectRemoved(AssetTags, GrantedTags, EffectRemoved.Handle);
}

void UVyraCommonActivatableWidget_WithASC::OnAnyGameplayTagChanged(const FGameplayTag GameplayTag, const int32 NewCount)
{
	HandleGameplayTagChange(GameplayTag, NewCount);
}

void UVyraCommonActivatableWidget_WithASC::OnAbilityCommitted(UGameplayAbility* ActivatedAbility)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!IsValid(ActivatedAbility))
	{
		GSC_LOG(Warning, TEXT("UVyraCommonActivatableWidget_WithASC::OnAbilityCommitted() Activated ability not valid"))
		return;
	}

	// Figure out cooldown
	const UGameplayEffect* CooldownGE = ActivatedAbility->GetCooldownGameplayEffect();
	if (!CooldownGE)
	{
		return;
	}

	if (!ActivatedAbility->IsInstantiated())
	{
		return;
	}

	const FGameplayTagContainer* CooldownTags = ActivatedAbility->GetCooldownTags();
	if (!CooldownTags || CooldownTags->Num() <= 0)
	{
		return;
	}

	const FGameplayAbilityActorInfo ActorInfo = ActivatedAbility->GetActorInfo();
	const FGameplayAbilitySpecHandle AbilitySpecHandle = ActivatedAbility->GetCurrentAbilitySpecHandle();

	float TimeRemaining = 0.f;
	float Duration = 0.f;
	ActivatedAbility->GetCooldownTimeRemainingAndDuration(AbilitySpecHandle, &ActorInfo, TimeRemaining, Duration);

	// Broadcast start of cooldown to HUD
	const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (AbilitySpec)
	{
		HandleCooldownStart(AbilitySpec->Ability, *CooldownTags, TimeRemaining, Duration);
	}

	// Register delegate to monitor any change to cooldown gameplay tag to be able to figure out when a cooldown expires
	TArray<FGameplayTag> GameplayTags;
	CooldownTags->GetGameplayTagArray(GameplayTags);
	for (const FGameplayTag GameplayTag : GameplayTags)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag).AddUObject(this, &UVyraCommonActivatableWidget_WithASC::OnCooldownGameplayTagChanged, AbilitySpecHandle, Duration);
		GameplayTagBoundToDelegates.AddUnique(GameplayTag);
	}
}

void UVyraCommonActivatableWidget_WithASC::OnCooldownGameplayTagChanged(const FGameplayTag GameplayTag, const int32 NewCount, FGameplayAbilitySpecHandle AbilitySpecHandle, float Duration)
{
	if (NewCount != 0)
	{
		return;
	}

	if (!AbilitySystemComponent)
	{
		return;
	}

	const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (!AbilitySpec)
	{
		// Ability might have been cleared when cooldown expires
		return;
	}

	const UGameplayAbility* Ability = AbilitySpec->Ability;

	// Broadcast cooldown expiration to HUD
	if (IsValid(Ability))
	{
		HandleCooldownEnd(AbilitySpec->Ability, GameplayTag, Duration);
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
}


void UVyraCommonActivatableWidget_WithASC::HandleGameplayEffectStackChange(const FGameplayTagContainer AssetTags, const FGameplayTagContainer GrantedTags, const FActiveGameplayEffectHandle ActiveHandle, const int32 NewStackCount, const int32 OldStackCount)
{
	OnGameplayEffectStackChange(AssetTags, GrantedTags, ActiveHandle, NewStackCount, OldStackCount);
}

void UVyraCommonActivatableWidget_WithASC::HandleGameplayEffectTimeChange(const FGameplayTagContainer AssetTags, const FGameplayTagContainer GrantedTags, const FActiveGameplayEffectHandle ActiveHandle, const float NewStartTime, const float NewDuration)
{
	OnGameplayEffectTimeChange(AssetTags, GrantedTags, ActiveHandle, NewStartTime, NewDuration);
}

void UVyraCommonActivatableWidget_WithASC::HandleGameplayEffectAdded(const FGameplayTagContainer AssetTags, const FGameplayTagContainer GrantedTags, const FActiveGameplayEffectHandle ActiveHandle)
{
	OnGameplayEffectAdded(AssetTags, GrantedTags, ActiveHandle, GetGameplayEffectUIData(ActiveHandle));
}

void UVyraCommonActivatableWidget_WithASC::HandleGameplayEffectRemoved(const FGameplayTagContainer AssetTags, const FGameplayTagContainer GrantedTags, const FActiveGameplayEffectHandle ActiveHandle)
{
	OnGameplayEffectRemoved(AssetTags, GrantedTags, ActiveHandle, GetGameplayEffectUIData(ActiveHandle));
}

void UVyraCommonActivatableWidget_WithASC::HandleGameplayTagChange(const FGameplayTag GameplayTag, const int32 NewTagCount)
{
	OnGameplayTagChange(GameplayTag, NewTagCount);
}

void UVyraCommonActivatableWidget_WithASC::HandleCooldownStart(UGameplayAbility* Ability, const FGameplayTagContainer CooldownTags, const float TimeRemaining, const float Duration)
{
	OnCooldownStart(Ability, CooldownTags, TimeRemaining, Duration);
}

void UVyraCommonActivatableWidget_WithASC::HandleCooldownEnd(UGameplayAbility* Ability, const FGameplayTag CooldownTag, const float Duration)
{
	OnCooldownEnd(Ability, CooldownTag, Duration);
}

FVyraGameplayEffectUIData UVyraCommonActivatableWidget_WithASC::GetGameplayEffectUIData(const FActiveGameplayEffectHandle ActiveHandle)
{
	return FVyraGameplayEffectUIData(
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectStartTime(ActiveHandle),
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectTotalDuration(ActiveHandle),
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectExpectedEndTime(ActiveHandle),
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectStackCount(ActiveHandle),
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectStackLimitCount(ActiveHandle)
	);
}
