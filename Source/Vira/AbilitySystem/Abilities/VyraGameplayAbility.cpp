// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility.h"

#include "CommonInputSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "DataAsset_AbilityGameplayTagStack.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "Vira/Messages/VyraVerbMessageHelpers.h"
#include "Vira/System/VyraGameInstance.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"
#include "Vira/UI/Player/HUD/VyraWidget_PlayerHUD_PC.h"

#define LOCTEXT_NAMESPACE "GameplayAbility"

UVyraGameplayAbility::UVyraGameplayAbility(): PlayerStateCharacter(nullptr), AbilityGameplayTagStack(nullptr),
                                              MontageToPlay(nullptr),
                                              CommonInputSubsystem(nullptr), PlayerController(nullptr)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#if WITH_EDITOR
EDataValidationResult UVyraGameplayAbility::IsDataValid(class FDataValidationContext& Context) const
{
	if (Super::IsDataValid(Context) == EDataValidationResult::Valid && AbilityGameplayTagStack)
	{
		return EDataValidationResult::Valid;
	}
	return EDataValidationResult::Invalid;
}
#endif

void UVyraGameplayAbility::OnGameplayTagStackUpdated_Implementation(FGameplayTag UpdatedTag, const int32 NewTagCount)
{
	if (UpdatedTag.IsValid() && UpdatedTag == GetAbilityLevelTag())
	{
		UpdateAbilityLevel(NewTagCount);
		OnAbilityLevelChanged.Broadcast(NewTagCount);
	}
}



void UVyraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AVyraPlayerStateCharacter* PSC = Cast<AVyraPlayerStateCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerStateCharacter = PSC;

		if (APlayerController* PC = UVyraVerbMessageHelpers::GetPlayerControllerFromObject(PlayerStateCharacter))
		{
			PlayerController = PC;

			CommonInputSubsystem = PC->GetLocalPlayer()->GetSubsystem<UCommonInputSubsystem>();
		}

		if (UVyraAbilitySystemComponent* ASC = GetVyraAbilitySystemComponent())
		{
			if (GetAbilityLevelTag().IsValid())
			{
				const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(GetAbilityLevelTag().GetTagName());
				if (const int32 Level =  ASC->GetGameplayTagStackCount(Tag); Level != -1)
				{
					UpdateAbilityLevel(Level);
				}
			}
			else
			{
				UpdateAbilityLevel(1);
			}

			ASC->GetGameplayTagStackComponent()->OnTagStackChanged.AddDynamic(this, &UVyraGameplayAbility::OnGameplayTagStackUpdated);
		}
	}
}

void UVyraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// If the ability cannot be committed, end it here
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
}

void UVyraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UVyraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                              const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bCanActivateAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (bCanActivateAbility == false)
	{
		if (UVyraGameInstance* VyraGameInstance = UVyraBlueprintFunctionLibrary::GetVyraGameInstance(GetAvatarActorFromActorInfo()))
		{
			if (UGameplayMessageSubsystem* Subsystem = VyraGameInstance->GetSubsystem<UGameplayMessageSubsystem>())
			{
				FVyraFailureMessage FailureMessage;
		
				for (FGameplayTag Tag : OptionalRelevantTags->GetGameplayTagArray())
				{
					if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.CanActivateAbility")))
					{
					}
					else if (Tag == FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.Cooldown")))
					{
						FText ErrorMessage = FText::Format(
							LOCTEXT("AbilityOnCooldownKey", "{0} is on cooldown!"), 
							AbilityInfo.Name.IsEmpty() ? FText::FromString("UnnamedAbility") : AbilityInfo.Name
						);

						FailureMessage.FailureMessage = ErrorMessage;
						Subsystem->BroadcastMessage(FGameplayTag::RequestGameplayTag("UIMessaging.FailureMessage", false), FailureMessage);
					}
					else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.Cost"))))
					{
						FText ErrorMessage = FText::Format(
							LOCTEXT("AbilityNotEnoughResourcesKey", "Not enough resources to use {0}!"),
							AbilityInfo.Name.IsEmpty() ? FText::FromString("UnnamedAbility") : AbilityInfo.Name
						);
						
						FailureMessage.FailureMessage = ErrorMessage;
						Subsystem->BroadcastMessage(FGameplayTag::RequestGameplayTag("UIMessaging.FailureMessage", false), FailureMessage);
					}
					else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.TagsBlocked"))))
					{
					}
					else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.TagsMissing"))))
					{
					}
					else
					{
					}
				}
			}
		}
	}
	return bCanActivateAbility;
}

bool UVyraGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ensure(ActorInfo))
	{
		return true;
	}

	if (!AbilityGameplayTagStack)
	{
		return true;
	}
	
	const FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(AbilityGameplayTagStack->GetCooldownGameplayTag().GetTagName());
	if (CooldownTag.IsValid())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			if (AbilitySystemComponent->HasMatchingGameplayTag(CooldownTag))
			{
				if (OptionalRelevantTags)
				{
					const FGameplayTag& FailCooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;
					if (FailCooldownTag.IsValid())
					{
						OptionalRelevantTags->AddTag(FailCooldownTag);
					}

					// Let the caller know which tags were blocking
					FGameplayTagContainer RelevantTags;
					RelevantTags.AddTag(CooldownTag);
					OptionalRelevantTags->AppendMatchingTags(AbilitySystemComponent->GetOwnedGameplayTags(), RelevantTags);
				}

				return false;
			}
		}
	}
	return true;
}

void UVyraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (CooldownGameplayEffectClass)
	{
		FGameplayEffectSpecHandle CooldownSpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, GetAbilityLevel());

		if (CooldownSpecHandle.IsValid() && AbilityGameplayTagStack)
		{
			const FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(AbilityGameplayTagStack->GetCooldownGameplayTag().GetTagName());

			//CooldownSpecHandle.Data->DynamicGrantedTags.AddTag(CooldownTag);

			CooldownSpecHandle.Data->DynamicGrantedTags.AddTag(CooldownTag);
			const int32 CooldownTime = GetGameplayTagStackCount(CooldownTag);
			if (CooldownTime > 0.f)
			{
				// Use FMath::Max for the possibility of the tag not being set. Negative cooldowns are not supported.
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(CooldownSpecHandle, FGameplayTag::RequestGameplayTag("SetByCaller.Magnitude.Duration"), FMath::Max(CooldownTime, 0.f));
			
				// Apply the cooldown effect to the owner
				ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CooldownSpecHandle);
			}
		}
	}
}

const FGameplayTagContainer* UVyraGameplayAbility::GetCooldownTags() const
{
	if (AbilityGameplayTagStack)
	{
		return new FGameplayTagContainer(AbilityGameplayTagStack->GetCooldownGameplayTag());
	}

	return Super::GetCooldownTags();
}

float UVyraGameplayAbility::GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const
{
	const UAbilitySystemComponent* const ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC)
	{
		const FGameplayTagContainer* CooldownTags = GetCooldownTags();
		if (CooldownTags && CooldownTags->Num() > 0)
		{
			FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(*CooldownTags);
			TArray< float > Durations = ASC->GetActiveEffectsTimeRemaining(Query);
			if (Durations.Num() > 0)
			{
				Durations.Sort();
				return Durations[Durations.Num() - 1];
			}
		}
	}

	return 0.f;
}

bool UVyraGameplayAbility::GetMouseLocation(FVector& HitLocation, const float TraceDistance) const
{

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Define the start and end points for the trace
		const FVector Start = WorldLocation;
		const FVector End = Start + (WorldDirection * TraceDistance);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetAvatarActorFromActorInfo());

		// Perform the line trace
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel4, CollisionParams))
		{
			HitLocation = HitResult.ImpactPoint; // Location where the trace hit
			return true;
		}
	}
	return false;
}

float UVyraGameplayAbility::GetCastSpeed() const
{
	bool bFoundAttribute = false;
	const float CastSpeed = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UCombatAttributeSet::GetCastSpeedAttribute(), bFoundAttribute);

	return  bFoundAttribute ? CastSpeed : 1.f;
}

float UVyraGameplayAbility::GetAttackSpeed() const
{
	bool bFoundAttribute = false;
	const float AttackSpeed = UAbilitySystemBlueprintLibrary::GetFloatAttribute(GetAvatarActorFromActorInfo(), UCombatAttributeSet::GetAttackSpeedAttribute(), bFoundAttribute);

	return  bFoundAttribute ? AttackSpeed : 1.f;
}

#undef LOCTEXT_NAMESPACE
