// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Vira/AbilitySystem/VyraAbilitySystemComponent.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"


#if WITH_EDITOR
void UPowerUpDataAsset::RefreshPowerUpEntries()
{
    PowerUpEntries.Empty();

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().ScanPathsSynchronous({ PowerUpDirectory.Path });

    FARFilter Filter;
    Filter.ClassPaths.Add(UPowerUpDefinition::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;
    Filter.PackagePaths.Add(FName(*PowerUpDirectory.Path));

    AssetRegistryModule.Get().GetAssets(Filter, AssetData);

    for (const FAssetData& Data : AssetData)
    {
        if (UPowerUpDefinition* Entry = Cast<UPowerUpDefinition>(Data.GetAsset()))
        {
            PowerUpEntries.Add(Entry);
        }
    }

    MarkPackageDirty();
}
#endif

UPowerUpComponent::UPowerUpComponent(): PowerUpDataAsset(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
}

TArray<FPowerUpSelection> UPowerUpComponent::RollForPowerUps(UAbilitySystemComponent* AbilitySystemComponent, int32 Quantity, float QualityIncrease)
{
    TArray<FPowerUpSelection> RolledPowerUps;
    if (!PowerUpDataAsset || Quantity <= 0) return RolledPowerUps;

    // Get player's active ability tags
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

    TArray<UPowerUpDefinition*> AllPowerUps = PowerUpDataAsset->PowerUpEntries;
    TArray<UPowerUpDefinition*> SelectedNonConsumables;

    for (int32 i = 0; i < Quantity; ++i)
    {
        // Roll quality first with fallback mechanism
        EPowerUpQuality CurrentQuality = RollForQuality(QualityIncrease);
        int32 FallbackAttempts = 4;
        TArray<UPowerUpDefinition*> EligiblePowerUps;

        while (FallbackAttempts-- > 0)
        {
            // Filter power-ups for current quality
            EligiblePowerUps.Empty();
            for (UPowerUpDefinition* PowerUp : AllPowerUps)
            {
                // Check quality availability
                if (!PowerUp->QualityConfigs.Contains(CurrentQuality))
                    continue;

                // Check required tags
                if (!PowerUp->RequiredTags.IsEmpty() && !ActiveAbilityTags.HasAll(PowerUp->RequiredTags))
                    continue;

                // Check if already selected in this roll (non-consumables only)
                if (!PowerUp->bIsConsumableEffect && SelectedNonConsumables.Contains(PowerUp))
                    continue;

                // Check existing instances
                FActivePowerUpInstance* Existing = ActivePowerUps.FindByPredicate(
                    [PowerUp](const FActivePowerUpInstance& Inst) {
                        return Inst.Definition == PowerUp;
                    });

                if (PowerUp->bIsConsumableEffect)
                {
                    // Always allow consumables
                    EligiblePowerUps.Add(PowerUp);
                }
                else if (!Existing)
                {
                    // Allow new power-ups
                    EligiblePowerUps.Add(PowerUp);
                }
                else if (CurrentQuality > Existing->CurrentQuality)
                {
                    // Allow upgrades
                    EligiblePowerUps.Add(PowerUp);
                }
            }

            if (EligiblePowerUps.Num() > 0) break;
            
            // Fallback to lower quality
            CurrentQuality = static_cast<EPowerUpQuality>(static_cast<int32>(CurrentQuality) - 1);
            if (CurrentQuality < EPowerUpQuality::Common) break;
        }

        if (EligiblePowerUps.Num() == 0) continue;

        // Calculate weights for remaining power-ups
        TArray<float> Weights;
        float TotalWeight = 0.f;
        for (UPowerUpDefinition* PowerUp : EligiblePowerUps)
        {
            const FPowerUpQualityConfig& Config = PowerUp->QualityConfigs[CurrentQuality];
            Weights.Add(Config.SelectionWeight);
            TotalWeight += Config.SelectionWeight;
        }

        // Select random power-up
        const float Roll = FMath::FRand() * TotalWeight;
        float Accumulated = 0.f;
        int32 SelectedIndex = 0;
        
        for (; SelectedIndex < Weights.Num(); SelectedIndex++)
        {
            Accumulated += Weights[SelectedIndex];
            if (Roll <= Accumulated) break;
        }

        UPowerUpDefinition* SelectedPowerUp = EligiblePowerUps[FMath::Min(SelectedIndex, EligiblePowerUps.Num()-1)];

        // Add to results
        FPowerUpSelection Selection;
        Selection.PowerUp = SelectedPowerUp;
        Selection.Quality = CurrentQuality;
        RolledPowerUps.Add(Selection);

        // Track non-consumables
        if (!SelectedPowerUp->bIsConsumableEffect)
        {
            SelectedNonConsumables.Add(SelectedPowerUp);
        }
    }

    return RolledPowerUps;
}

EPowerUpQuality UPowerUpComponent::RollForQuality(float QualityIncrease)
{
    TArray<TPair<EPowerUpQuality, float>> QualityWeights;
    float TotalWeight = 0.0f;

    // Calculate weights based on progression
    for (const auto& Entry : PowerUpDataAsset->QualityProbabilityTable)
    {
        const float CurveValue = Entry.Value.ProbabilityCurve.GetRichCurveConst()->Eval(QualityIncrease);
        const float Weight = Entry.Value.BaseProbability * CurveValue;
        QualityWeights.Add(TPair<EPowerUpQuality, float>(Entry.Key, Weight));
        TotalWeight += Weight;
    }

    // Normalized random selection
    const float RandomRoll = FMath::FRand() * TotalWeight;
    float Accumulated = 0.0f;

    for (const auto& Entry : QualityWeights)
    {
        Accumulated += Entry.Value;
        if (RandomRoll <= Accumulated)
        {
            return Entry.Key;
        }
    }

    // Fallback to lowest quality
    return EPowerUpQuality::Common;
}

int32 UPowerUpComponent::GetAbilityLevel(const UPowerUpDefinition* PowerUpData)
{
    return static_cast<int32>(PowerUpData->QualityConfigs.begin()->Key) + 1;
}

void UPowerUpComponent::ApplyPowerUp(UAbilitySystemComponent* AbilitySystemComponent, UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality)
{
    if (!PowerUpDef || !AbilitySystemComponent) return;

    // Find existing instance
    FActivePowerUpInstance* ExistingInstance = ActivePowerUps.FindByPredicate(
        [PowerUpDef](const FActivePowerUpInstance& Inst) {
            return Inst.Definition == PowerUpDef;
        });

    if (ExistingInstance)
    {
        HandleQualityUpgrade(*ExistingInstance, Quality, AbilitySystemComponent);
    }
    else
    {
        ApplyNewPowerUp(PowerUpDef, Quality, AbilitySystemComponent);
    }
}

void UPowerUpComponent::HandleQualityUpgrade(FActivePowerUpInstance& Instance, EPowerUpQuality NewQuality, UAbilitySystemComponent* ASC)
{
    if (static_cast<int32>(NewQuality) > static_cast<int32>(Instance.CurrentQuality))
    {
        // Remove old effects
        if (const FPowerUpQualityConfig* OldConfig = Instance.Definition->QualityConfigs.Find(Instance.CurrentQuality))
        {
            //RemoveQualityEffects(OldConfig, ASC);
        }

        // Apply new effects
        if (const FPowerUpQualityConfig* NewConfig = Instance.Definition->QualityConfigs.Find(NewQuality))
        {
            ApplyQualityEffects(NewConfig, ASC, static_cast<int32>(NewQuality) + 1);
            Instance.CurrentQuality = NewQuality;
        }
    }
    Instance.CurrentStack++;
}

void UPowerUpComponent::ApplyNewPowerUp(UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality, UAbilitySystemComponent* ASC)
{
    if (const FPowerUpQualityConfig* Config = PowerUpDef->QualityConfigs.Find(Quality))
    {
        FActivePowerUpInstance NewInstance;
        NewInstance.Definition = PowerUpDef;
        NewInstance.CurrentQuality = Quality;
        
        ApplyQualityEffects(Config, ASC, static_cast<int32>(Quality) + 1);

        if (!PowerUpDef->bIsConsumableEffect)
        {
            ActivePowerUps.Add(NewInstance);
        }
    }
}


void UPowerUpComponent::ApplyQualityEffects(const FPowerUpQualityConfig* Config, UAbilitySystemComponent* AbilitySystemComponent, const int32 Level)
{
    if (!Config || !AbilitySystemComponent) return;

    UVyraAbilitySystemComponent* VyraAbilitySystemComponent = Cast<UVyraAbilitySystemComponent>(AbilitySystemComponent);
    if (!VyraAbilitySystemComponent) return;

    // Apply gameplay effects
    for (auto EffectClass : Config->GameplayEffects)
    {
        if (EffectClass)
        {
            // Remove any active gameplay effects of the same class
            const FActiveGameplayEffectsContainer& ActiveEffects = VyraAbilitySystemComponent->GetActiveGameplayEffects();
            TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = ActiveEffects.GetAllActiveEffectHandles();
            for (auto EffectHandle : ActiveEffectHandles)
            {
                const FActiveGameplayEffect* ActiveEffect = ActiveEffects.GetActiveGameplayEffect(EffectHandle);
                if (ActiveEffect && ActiveEffect->Spec.Def->GetClass() == EffectClass)
                {
                    VyraAbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
                }
            }
            
            FGameplayEffectSpecHandle EffectSpecHandle = VyraAbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, VyraAbilitySystemComponent->MakeEffectContext());
            if (EffectSpecHandle.Data.IsValid())
            {
                VyraAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
            }
        }
    }

    // Grant abilities
    for (auto AbilityClass : Config->Abilities)
    {
        if (AbilityClass)
        {
            // If we already have the ability, we update its level.
            if (FGameplayAbilitySpec* ExistingAbilitySpec = VyraAbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass))
            {
                if (ExistingAbilitySpec->Level < Level)
                {
                    ExistingAbilitySpec->Level = Level;
                    
                    bool bIsInstanced = false;
                    if (const UGameplayAbility* GameplayAbility = UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(VyraAbilitySystemComponent, ExistingAbilitySpec->Handle, bIsInstanced))
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
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level, 0);
                VyraAbilitySystemComponent->GiveAbility(AbilitySpec);
            }
        }
    }

    // Add tag stacks
    for (auto& TagStack : Config->TagStacks)
    {
        const float StackCount = TagStack.GetStackCount();
        if (StackCount > 0)
        {
            VyraAbilitySystemComponent->RemoveGameplayTagStack(TagStack.GetTag(), StackCount);
        }
        else
        {
            VyraAbilitySystemComponent->AddGameplayTagStack(TagStack.GetTag(), StackCount);
        }
    }
}

void UPowerUpComponent::HandleExistingPowerUp(FActivePowerUpInstance& Instance, EPowerUpQuality NewQuality, UAbilitySystemComponent* AbilitySystemComponent)
{
    const EPowerUpQuality CurrentQuality = Instance.CurrentQuality;

    const int32 Level = GetAbilityLevel(Instance.Definition.Get());

    // Only upgrade if new quality is better
    if (static_cast<int32>(NewQuality) > static_cast<int32>(CurrentQuality))
    {
        // Remove old quality effects
        if (const FPowerUpQualityConfig* OldConfig = Instance.Definition->QualityConfigs.Find(CurrentQuality))
        {
            // Implement logic to remove old effects if needed
            // We do it in ApplyQualityEffects already, can decouple it if needed.
        }

        // Apply new quality effects
        if (const FPowerUpQualityConfig* NewConfig = Instance.Definition->QualityConfigs.Find(NewQuality))
        {
            ApplyQualityEffects(NewConfig, AbilitySystemComponent, Level);
        }

        Instance.CurrentQuality = NewQuality;
    }

    // Handle stacking logic
    Instance.CurrentStack++;
}

void UPowerUpComponent::HandleNewPowerUp(UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality, UAbilitySystemComponent* AbilitySystemComponent)
{
    const FPowerUpQualityConfig* QualityConfig = PowerUpDef->QualityConfigs.Find(Quality);
    if (!QualityConfig) return;

    // Create new instance
    FActivePowerUpInstance NewInstance;
    NewInstance.Definition = PowerUpDef;
    NewInstance.CurrentQuality = Quality;

    const int32 Level = GetAbilityLevel(PowerUpDef);

    // Apply quality-specific effects
    ApplyQualityEffects(QualityConfig, AbilitySystemComponent, Level);

    if (!PowerUpDef->bIsConsumableEffect)
    {
        ActivePowerUps.Add(NewInstance);
    }
}