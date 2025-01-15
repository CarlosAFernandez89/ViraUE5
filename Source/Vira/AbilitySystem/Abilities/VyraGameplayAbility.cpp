// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraGameplayAbility.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "Vira/System/VyraGameInstance.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"
#include "Vira/UI/Player/HUD/VyraWidget_PlayerHUD_PC.h"

#define LOCTEXT_NAMESPACE "GameplayAbility"

UVyraGameplayAbility::UVyraGameplayAbility(): PlayerStateCharacter(nullptr), AbilityInfo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVyraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	PlayerStateCharacter = Cast<AVyraPlayerStateCharacter>(GetAvatarActorFromActorInfo());
	
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
							FText::FromString(AbilityInfo.Name.IsEmpty() ? TEXT("UnnamedAbility") : AbilityInfo.Name)
						);

						FailureMessage.FailureMessage = ErrorMessage;
						Subsystem->BroadcastMessage(FGameplayTag::RequestGameplayTag("UIMessaging.FailureMessage", false), FailureMessage);
					}
					else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Vyra.ActivateFail.Cost"))))
					{
						FText ErrorMessage = FText::Format(
							LOCTEXT("AbilityNotEnoughResourcesKey", "Not enough resources to use {0}!"),
							FText::FromString(AbilityInfo.Name.IsEmpty() ? TEXT("UnnamedAbility") : AbilityInfo.Name)
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

#undef LOCTEXT_NAMESPACE