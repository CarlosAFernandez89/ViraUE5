// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Vira/System/GameplayTagStack.h"
#include "PowerUpComponent.generated.h"


class UAbilitySystemComponent;
class UPowerUpDefinition;

UENUM(BlueprintType)
enum class EPowerUpQuality : uint8
{
	Common			UMETA(DisplayName = "Common"),
	Rare			UMETA(DisplayName = "Rare"),
	Epic			UMETA(DisplayName = "Epic"),
	Legendary       UMETA(DisplayName = "Legendary"),
	Mythical		UMETA(DisplayName = "Mythical"),
	END
};

USTRUCT(BlueprintType)
struct FPowerUpSelection
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    UPowerUpDefinition* PowerUp;
    
    UPROPERTY(BlueprintReadOnly)
    EPowerUpQuality Quality;
};

USTRUCT(BlueprintType)
struct FPowerUpQualityInfo
{
    GENERATED_BODY()

    // Base probability for this quality
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseProbability = 0.3f;

    // Curve to modify probability based on progression
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRuntimeFloatCurve ProbabilityCurve;
};

USTRUCT(BlueprintType)
struct FPowerUpQualityConfig
{
    GENERATED_BODY()

    // Quality-specific effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class UGameplayEffect>> GameplayEffects;

    // Quality-specific abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class UGameplayAbility>> Abilities;

    // Quality-specific tag stacks
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGameplayTagStack> TagStacks;

    // Visual representation
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor DisplayColor = FLinearColor::White;

    // Weight for random selection
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SelectionWeight = 1.0f;
};


USTRUCT(BlueprintType)
struct FPowerUpEffectInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FText EffectName;

    UPROPERTY(BlueprintReadOnly)
    FText EffectDescription;

    UPROPERTY(BlueprintReadOnly)
    TMap<FGameplayAttribute, float> AttributeModifiers;
};

USTRUCT(BlueprintType)
struct FPowerUpUIData
{
    GENERATED_BODY()

    // Basic Information
    UPROPERTY(BlueprintReadOnly)
    FText DisplayName = FText();
    
    UPROPERTY(BlueprintReadOnly)
    UTexture2D* Icon = nullptr;
    
    UPROPERTY(BlueprintReadOnly)
    FText BaseDescription;

    // Quality Information
    UPROPERTY(BlueprintReadOnly)
    EPowerUpQuality Quality = EPowerUpQuality::Common;
    
    UPROPERTY(BlueprintReadOnly)
    FLinearColor QualityColor = FLinearColor::White;

    // Effect Breakdown
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> NumericEffects;  // Formatted "Attribute: +Value"
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TagEffects;      // Formatted "Tag xCount"

    // Meta Information
    UPROPERTY(BlueprintReadOnly)
    bool bIsConsumable = false;
};

UCLASS(BlueprintType)
class UPowerUpDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Common properties across all qualities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    FText PowerUpName = FText();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    FText Description = FText();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    UTexture2D* Icon = nullptr;

    // Quality-specific configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    TMap<EPowerUpQuality, FPowerUpQualityConfig> QualityConfigs;

    // Base requirements
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    FGameplayTagContainer RequiredTags;

    // Whether this power-up is consumable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    bool bIsConsumableEffect = false;
    
    
    UFUNCTION(BlueprintCallable, Category = "PowerUp")
    FPowerUpUIData GetUIData(EPowerUpQuality Quality) const
    {
        FPowerUpUIData Data;
        Data.DisplayName = PowerUpName;
        Data.Icon = Icon;
        Data.BaseDescription = Description; // Set this in data asset
        Data.Quality = Quality;
        Data.bIsConsumable = bIsConsumableEffect;
        
        switch (Quality)
        {
        case EPowerUpQuality::Common: Data.QualityColor = FLinearColor(0.769, 0.769, 0.769);
            break;
        case EPowerUpQuality::Rare: Data.QualityColor = FLinearColor(0.141, 0.431, 1);
            break;
        case EPowerUpQuality::Epic: Data.QualityColor = FLinearColor(0.514, 0, 0.922);
            break;
        case EPowerUpQuality::Legendary: Data.QualityColor = FLinearColor(0.929, 0.878, 0);
            break;
        case EPowerUpQuality::Mythical: Data.QualityColor = FLinearColor(0.561, 0, 0);
            break;
        case EPowerUpQuality::END:
            break;
        }

        
        if (const FPowerUpQualityConfig* Config = QualityConfigs.Find(Quality))
        {
            // Process numeric effects
            for (TSubclassOf<UGameplayEffect> EffectClass : Config->GameplayEffects)
            {
                if (const UGameplayEffect* EffectCDO = EffectClass.GetDefaultObject())
                {
                    for (const FGameplayModifierInfo& Modifier : EffectCDO->Modifiers)
                    {
                        // Function to add a space before each capital letter after the first character.
                        auto InsertSpacesBeforeCaps = [](const FString& Input)
                        {
                            if (Input.Len() <= 1) return Input;
                                
                            TStringBuilder<256> Output;
                            Output.AppendChar(Input[0]);

                            for (int32 i = 1; i < Input.Len(); ++i)
                            {
                                if (FChar::IsUpper(Input[i]))
                                {
                                    Output.AppendChar(' ');
                                }
                                Output.AppendChar(Input[i]);
                            }

                            return FString(Output);
                        };
                        
                        float Magnitude = 0.f;
                        if (Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(static_cast<int32>(Quality) + 1, Magnitude))
                        {
                            const FString AttrName = Modifier.Attribute.GetName();
                            Data.NumericEffects.Add(FString::Printf(TEXT("%s: %+.1f"), 
                                                                    *InsertSpacesBeforeCaps(AttrName), 
                                                                    Magnitude));
                        }
                    }
                }
            }

            // Process tag stacks
            for (const FGameplayTagStack& TagStack : Config->TagStacks)
            {
                if (!TagStack.TagUserFacingName.IsNone())
                {
                    Data.TagEffects.Add(FString::Printf(TEXT("%s %.2f"),
                                                        *TagStack.TagUserFacingName.ToString(),
                                                        TagStack.GetStackCount()));
                }
            }
        }


        return Data;
    }
};

USTRUCT(BlueprintType)
struct FActivePowerUpInstance
{
    GENERATED_BODY()

    // Reference to the power-up definition
    UPROPERTY()
    TWeakObjectPtr<UPowerUpDefinition> Definition;

    // Current quality of this instance
    UPROPERTY(BlueprintReadOnly)
    EPowerUpQuality CurrentQuality = EPowerUpQuality::Common;

    // Any instance-specific data
    UPROPERTY()
    int32 CurrentStack = 1;
};

UCLASS(BlueprintType)
class UPowerUpDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Directory containing power-up definitions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    FDirectoryPath PowerUpDirectory;

    // List of all power-up definitions
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PowerUp")
    TArray<UPowerUpDefinition*> PowerUpEntries;

    // Quality probability table
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
    TMap<EPowerUpQuality, FPowerUpQualityInfo> QualityProbabilityTable;

#if WITH_EDITOR
    // Refresh power-up entries from the directory
    UFUNCTION(CallInEditor, Category = "PowerUp")
    void RefreshPowerUpEntries();
#endif
};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRA_API UPowerUpComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UPowerUpComponent();

protected:
    // Rolls for power-ups based on the current game state
    UFUNCTION(BlueprintCallable, Category = "PowerUp")
    virtual TArray<FPowerUpSelection> RollForPowerUps(UAbilitySystemComponent* AbilitySystemComponent, int32 Quantity = 3, float QualityIncrease = 0.f);

    // Rolls for a specific quality based on progression
    UFUNCTION(Category = "PowerUp")
    virtual EPowerUpQuality RollForQuality(float QualityIncrease);

    // Gets the ability level for a power-up
    UFUNCTION(Category = "PowerUp")
    virtual int32 GetAbilityLevel(const UPowerUpDefinition* PowerUpData);

    // Applies a power-up to the player
    UFUNCTION(BlueprintCallable, Category = "PowerUp")
    virtual void ApplyPowerUp(UAbilitySystemComponent* AbilitySystemComponent, UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality);
    void HandleQualityUpgrade(FActivePowerUpInstance& Instance, EPowerUpQuality NewQuality,
                              UAbilitySystemComponent* ASC);
    void ApplyNewPowerUp(UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality, UAbilitySystemComponent* ASC);

private:
    // Applies quality-specific effects
    void ApplyQualityEffects(const FPowerUpQualityConfig* Config, UAbilitySystemComponent* AbilitySystemComponent, const int32 Level);

    // Handles upgrading an existing power-up
    void HandleExistingPowerUp(FActivePowerUpInstance& Instance, EPowerUpQuality NewQuality, UAbilitySystemComponent* AbilitySystemComponent);

    // Handles adding a new power-up
    void HandleNewPowerUp(UPowerUpDefinition* PowerUpDef, EPowerUpQuality Quality, UAbilitySystemComponent* AbilitySystemComponent);

protected:
    // Data asset containing all power-up definitions
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PowerUp")
    UPowerUpDataAsset* PowerUpDataAsset;

    // List of active power-ups
    UPROPERTY()
    TArray<FActivePowerUpInstance> ActivePowerUps;
};


