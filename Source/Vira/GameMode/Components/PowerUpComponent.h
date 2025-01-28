// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Vira/System/GameplayTagStack.h"
#include "PowerUpComponent.generated.h"


class UAbilitySystemComponent;


UENUM(BlueprintType)
enum class EPowerUpQuality : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
	Rare			UMETA(DisplayName = "Rare"),
	Epic			UMETA(DisplayName = "Epic"),
	Legendary       UMETA(DisplayName = "Legendary"),
	Mythical		UMETA(DisplayName = "Mythical"),
	END
};

USTRUCT(BlueprintType)
struct FPowerUpQuality
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpQuality Quality = EPowerUpQuality::Normal;
	
	UPROPERTY(BlueprintReadOnly, Category = "PowerUpQuality")
	FLinearColor Color = FLinearColor::White;

	FPowerUpQuality()
	{
		UpdateColor();
	}

	FLinearColor GetColor() const
	{
		FLinearColor WantedColor = FLinearColor::White;
		switch (Quality)
		{
		case EPowerUpQuality::Rare:
			WantedColor = FLinearColor(0.f,0.f,1.f);
			break;
		case EPowerUpQuality::Epic:
			WantedColor = FLinearColor(0.52f, 0.01f,0.49f);
			break;
		case EPowerUpQuality::Legendary:
			WantedColor = FLinearColor(1.f,1.f,0.f);
			break;
		case EPowerUpQuality::Mythical:
			WantedColor = FLinearColor(1.f,0.1f,0.1f);
			break;
		default:
			WantedColor = FLinearColor(0.75f,0.75f,0.75f);
			break;
		}

		return WantedColor;
	}
	
	void UpdateColor()
	{
		switch (Quality)
		{
		case EPowerUpQuality::Rare:
			Color = FLinearColor(0.f,0.f,1.f);
			break;
		case EPowerUpQuality::Epic:
			Color = FLinearColor(0.52f, 0.01f,0.49f);
			break;
		case EPowerUpQuality::Legendary:
			Color = FLinearColor(1.f,1.f,0.f);
			break;
		case EPowerUpQuality::Mythical:
			Color = FLinearColor(1.f,0.1f,0.1f);
			break;
		default:
			Color = FLinearColor(0.75f,0.75f,0.75f);
			break;
		}
	}

	void UpdateQuality(const EPowerUpQuality NewQuality)
	{
		Quality = NewQuality;
		UpdateColor();
	}
};

USTRUCT(BlueprintType)
struct FPowerUpQualityInfo
{
	GENERATED_BODY()

	// Base probability without any modifiers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quality")
	float BaseProbability = 0.3f;

	// Curve to modify probability based on game progression
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quality")
	FRuntimeFloatCurve ProbabilityCurve;
    
	// Visual representation data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quality")
	FLinearColor DisplayColor = FLinearColor::White;
    
	// Minimum quality level required to appear
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quality")
	int32 MinPlayerLevel = 0;
};

USTRUCT(BlueprintType)
struct FPowerUpData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	FName PowerUpName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|UserFacing")
	FText DisplayName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|UserFacing")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|UserFacing")
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|UserFacing")
	FPowerUpQuality Quality = FPowerUpQuality();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	FGameplayTagContainer RequiredAbilityTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> AbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|GameplayEffects")
	TArray<TSubclassOf<class UGameplayEffect>> GameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp|GameplayEffects")
	TArray<FGameplayTagStack> GameplayTagStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	bool bIsConsumableEffect = false;
	
	bool operator==(const FPowerUpData& Other) const
	{
		return PowerUpName == Other.PowerUpName;
	}
};

UCLASS()
class VIRA_API UPowerUpDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="PowerUp")
	TMap<EPowerUpQuality, FPowerUpQualityInfo> QualityProbabilityTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	TArray<FPowerUpData> PowerUps;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

		// Check if the Quality property was changed in any of the PowerUps
		if (PropertyName == GET_MEMBER_NAME_CHECKED(FPowerUpData, Quality))
		{
			for (FPowerUpData& PowerUp : PowerUps)
			{
				PowerUp.Quality.UpdateColor();
			}
		}
	}
#endif
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UPowerUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPowerUpComponent();

protected:

	UFUNCTION(BlueprintCallable, Category="PowerUp")
	virtual TArray<FPowerUpData> RollForPowerUps(UAbilitySystemComponent* AbilitySystemComponent, const int32 Quantity = 3, float QualityIncrease = 0.f);

	UFUNCTION(Category="PowerUp")
	virtual EPowerUpQuality RollForQuality(float QualityIncrease);

	UFUNCTION(Category="PowerUp")
	virtual int32 GetAbilityLevel(const FPowerUpData PowerUpData);

	UFUNCTION(BlueprintCallable, Category="PowerUp")
	virtual void ApplyPowerUpToPlayer(UAbilitySystemComponent* AbilitySystemComponent,const FPowerUpData PowerUpData);

private:

	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PowerUp")
	UPowerUpDataAsset* PowerUpDataAsset;

	UPROPERTY()
	TArray<FPowerUpData> ActivePowerUps;
	
};
