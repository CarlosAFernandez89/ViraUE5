// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"


// Sets default values for this component's properties
UPowerUpComponent::UPowerUpComponent(): PowerUpDataAsset(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FPowerUpData> UPowerUpComponent::RollForPowerUps(UAbilitySystemComponent* AbilitySystemComponent, const int32 Quantity, float QualityIncrease)
{
    TArray<FPowerUpData> RolledPowerUps;
    if (!PowerUpDataAsset) return RolledPowerUps;

    // 1. Get player's active ability tags
    FGameplayTagContainer ActiveAbilityTags;
    if (AbilitySystemComponent)
    {
        for (const FGameplayAbilitySpec& Ability : AbilitySystemComponent->GetActivatableAbilities())
        {
            if (UGameplayAbility* GameplayAbility = Ability.Ability.Get())
            {
                ActiveAbilityTags.AppendTags(GameplayAbility->GetAssetTags());
            }
        }
    }

    // 2. Filter and prepare possible powerups
    TArray<FPowerUpData> PossiblePowerUps;
    TMap<FName, EPowerUpQuality> ActivePowerUpMap;
    
    for (const FPowerUpData& PowerUp : ActivePowerUps)
    {
        ActivePowerUpMap.Add(PowerUp.PowerUpName, PowerUp.Quality.Quality);
    }

    for (const FPowerUpData& PowerUp : PowerUpDataAsset->PowerUps)
    {
        // Check required tags
        if (!PowerUp.RequiredAbilityTags.IsEmpty() && !ActiveAbilityTags.HasAll(PowerUp.RequiredAbilityTags))
            continue;

        // Filter out maxed non-consumables
        if (!PowerUp.bIsConsumableEffect)
        {
            if (const EPowerUpQuality* ExistingQuality = ActivePowerUpMap.Find(PowerUp.PowerUpName))
            {
                if (*ExistingQuality == EPowerUpQuality::Mythical) continue;
            }
        }

        // Filter out lower or equal quality power-ups (if already have)
        if (const EPowerUpQuality* ExistingQuality = ActivePowerUpMap.Find(PowerUp.PowerUpName))
        {
            if (*ExistingQuality <= PowerUp.Quality.Quality) continue;
        }

        if (UVyraAbilitySystemComponent* VyraASC = Cast<UVyraAbilitySystemComponent>(AbilitySystemComponent))
        {
            // Filter out stacks which already reached max
            bool bFilterByGameplayTagStack = false;
            for (FGameplayTagStack TagStack : PowerUp.GameplayTagStacks)
            {
                int32 MaxTagCount = VyraASC->GetGameplayTagStackComponent()->GetMaxTagStackCount(TagStack.GetTag());

                if (MaxTagCount <= 0)
                    continue;
                
                int32 CurrentTagCount = VyraASC->GetGameplayTagStackComponent()->GetTagStackCount(TagStack.GetTag());

                if (CurrentTagCount >= MaxTagCount)
                    bFilterByGameplayTagStack = true;
            }
            
            if (bFilterByGameplayTagStack) continue;
        }


        PossiblePowerUps.Add(PowerUp);
    }

    if (PossiblePowerUps.Num() == 0) return RolledPowerUps;

    // 3. Calculate dynamic weights using the Quality Probability Table
    TArray<float> CumulativeWeights;
    float TotalWeight = 0.0f;

    for (const FPowerUpData& PowerUp : PossiblePowerUps)
    {
        if (const FPowerUpQualityInfo* QualityInfo = PowerUpDataAsset->QualityProbabilityTable.Find(PowerUp.Quality.Quality))
        {
            const float CurveValue = QualityInfo->ProbabilityCurve.GetRichCurveConst()->Eval(QualityIncrease);
            const float FinalWeight = QualityInfo->BaseProbability * CurveValue;
            TotalWeight += FinalWeight;
            CumulativeWeights.Add(TotalWeight);
        }
        else
        {
            CumulativeWeights.Add(TotalWeight); // Default weight if no entry found
        }
    }

    // 4. Perform rolls
    for (int32 i = 0; i < Quantity; ++i)
    {
        if (PossiblePowerUps.Num() == 0) break;

        const float RolledNum = FMath::FRand() * TotalWeight;
        const int32 SelectedIndex = Algo::UpperBound(CumulativeWeights, RolledNum);

        if (SelectedIndex >= 0 && SelectedIndex < PossiblePowerUps.Num())
        {
            FPowerUpData SelectedPowerUp = PossiblePowerUps[SelectedIndex];

            // Handle upgrades for non-consumables
            if (!SelectedPowerUp.bIsConsumableEffect)
            {
                if (const EPowerUpQuality* ExistingQuality = ActivePowerUpMap.Find(SelectedPowerUp.PowerUpName))
                {
                    const int32 NewQuality = FMath::Min(
                        static_cast<int32>(*ExistingQuality) + 1,
                        static_cast<int32>(EPowerUpQuality::Mythical)
                    );
                    SelectedPowerUp.Quality.Quality = static_cast<EPowerUpQuality>(NewQuality);
                    SelectedPowerUp.Quality.UpdateColor();
                }
            }

            RolledPowerUps.Add(SelectedPowerUp);

            // We only remove non-consumables from the possible power-ups. you can get multiple of same consumable to choose from.
            if (!SelectedPowerUp.bIsConsumableEffect)
            {
                // Remove selected powerup to prevent duplicates in same roll
                PossiblePowerUps.RemoveAt(SelectedIndex);
                CumulativeWeights.RemoveAt(SelectedIndex, CumulativeWeights.Num() - SelectedIndex);
                TotalWeight = CumulativeWeights.Num() > 0 ? CumulativeWeights.Last() : 0.0f;
            }
        }
    }

    return RolledPowerUps;
}

EPowerUpQuality UPowerUpComponent::RollForQuality(float QualityIncrease)
{
    float TotalWeight = 0.0f;
    TArray<float> QualityWeights;
    TArray<EPowerUpQuality> QualityOrder;

    // Calculate dynamic weights
    for (const auto& Entry : PowerUpDataAsset->QualityProbabilityTable)
    {
        const EPowerUpQuality Quality = Entry.Key;
        const FPowerUpQualityInfo& Info = Entry.Value;

        const float CurveValue = Info.ProbabilityCurve.GetRichCurveConst()->Eval(QualityIncrease);
        const float FinalWeight = Info.BaseProbability * CurveValue;

        QualityWeights.Add(FinalWeight);
        QualityOrder.Add(Quality);
        TotalWeight += FinalWeight;
    }

    // Normalize and roll
    const float RandomRoll = FMath::FRand() * TotalWeight;
    float Accumulated = 0.0f;

    for (int32 i = 0; i < QualityWeights.Num(); ++i)
    {
        Accumulated += QualityWeights[i];
        if (RandomRoll <= Accumulated)
        {
            return QualityOrder[i];
        }
    }

    // Fallback to highest quality
    return EPowerUpQuality::Mythical;
}

int32 UPowerUpComponent::GetAbilityLevel(const FPowerUpData PowerUpData)
{
	return static_cast<int32>(PowerUpData.Quality.Quality) + 1;
}

void UPowerUpComponent::ApplyPowerUpToPlayer(UAbilitySystemComponent* AbilitySystemComponent,
                                             const FPowerUpData PowerUpData)
{
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent is null!"));
        return;
    }

    const int32 AbilityLevel = GetAbilityLevel(PowerUpData);

    // Handle Abilities
    for (auto Ability : PowerUpData.AbilityClass)
    {
        if (Ability)
        {
            // If we already have the ability, we update its level.
            if (FGameplayAbilitySpec* ExistingAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Ability))
            {
                if (ExistingAbilitySpec->Level < AbilityLevel)
                {
                    ExistingAbilitySpec->Level = AbilityLevel;
                    
                    bool bIsInstanced = false;
                    if (const UGameplayAbility* GameplayAbility = UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilitySystemComponent, ExistingAbilitySpec->Handle, bIsInstanced))
                    {
                        if (const UVyraGameplayAbility* VyraGameplayAbility = Cast<UVyraGameplayAbility>(GameplayAbility))
                        {
                            VyraGameplayAbility->OnAbilityLevelChanged.Broadcast(ExistingAbilitySpec->Level);
                        }
                    }
                }
            }
            else
            {
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel, 0);
                AbilitySystemComponent->GiveAbility(AbilitySpec);
            }
        }
    }

    // Handle Gameplay Effects
    for (auto GameplayEffect : PowerUpData.GameplayEffects)
    {
        if (GameplayEffect)
        {
            // Remove any active gameplay effects of the same class
            const FActiveGameplayEffectsContainer& ActiveEffects = AbilitySystemComponent->GetActiveGameplayEffects();
            TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = ActiveEffects.GetAllActiveEffectHandles();
            for (auto EffectHandle : ActiveEffectHandles)
            {
                const FActiveGameplayEffect* ActiveEffect = ActiveEffects.GetActiveGameplayEffect(EffectHandle);
                if (ActiveEffect && ActiveEffect->Spec.Def->GetClass() == GameplayEffect)
                {
                    AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
                }
            }

            // Apply new Gameplay Effect
            FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, AbilityLevel, AbilitySystemComponent->MakeEffectContext());
            if (EffectSpecHandle.Data.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create GameplayEffectSpec for %s"), *GameplayEffect->GetName());
            }
        }
    }

    // Handle GameplayTagStacks
    if (UVyraAbilitySystemComponent* VyraASC = Cast<UVyraAbilitySystemComponent>(AbilitySystemComponent))
    {
        int32 Size = PowerUpData.GameplayTagStacks.Num();

        TArray<FGameplayTag> AppliedTags; 
        for (int32 i = 0; i < Size; ++i)
        {
            const FGameplayTag Tag = PowerUpData.GameplayTagStacks[i].GetTag();
            if (AppliedTags.Contains(Tag)) break;
            
            const float StackCount = PowerUpData.GameplayTagStacks[i].GetStackCount();
            
            VyraASC->AddGameplayTagStack(Tag, StackCount);
            AppliedTags.Add(Tag);
            // Log a message to the screen
            if (GEngine)
            {
                FString DebugMessage = FString::Printf(TEXT("Added Tag Stack: Tag = %s, Count = %f, ArraySize = %d"), *Tag.ToString(), StackCount, Size);
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, DebugMessage);
            }
        }
    }

	if (PowerUpData.bIsConsumableEffect) return;
	
    // Update or add the power-up to the active list
    const int32 Index = ActivePowerUps.Find(PowerUpData);
    if (Index != INDEX_NONE)
    {
        ActivePowerUps[Index].Quality.UpdateQuality(static_cast<EPowerUpQuality>(AbilityLevel - 1));
    }
    else
    {
        ActivePowerUps.AddUnique(PowerUpData);
    }
}
