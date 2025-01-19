// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Vira/System/GameplayTagStack.h"
#include "GameplayTagStackComponent.generated.h"

class UDataAsset_AbilityGameplayTagStack;

USTRUCT(BlueprintType)
struct FGameplayAbilityTagStackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GameplayTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCount = -1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagStackChanged, FGameplayTag, ChangedTag, int32, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UGameplayTagStackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameplayTagStackComponent();

public:

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void InitializeNewStackTag(FGameplayTag Tag, const float CurrentCount, const float MaxCount);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void SetTagStack(FGameplayTag Tag, const float NewStackCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void AddToTagStack(FGameplayTag Tag, const float CountToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void RemoveTagStack(FGameplayTag Tag, const float CountToRemove);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	int32 GetTagStackCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	int32 GetMaxTagStackCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	bool HasTagStack(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	const FGameplayTagStackContainer& GetTagStackContainer() const
	{
		return TagStackContainer;
	}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void SetTagStackContainer(const FGameplayTagStackContainer& InContainer)
	{
		TagStackContainer = InContainer;
	}

private:
	int32 FindGameplayTagStackIndexByGameplayTag(const FGameplayTag& Tag) const;
	float ClampToMaxStackCount(FGameplayTag Tag, const float MaxCount) const;
	float ClampToMinStackCount(FGameplayTag Tag, const float MinCount) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UDataAsset_AbilityGameplayTagStack*> RelevantGameplayTagStacks;
	
	UPROPERTY(BlueprintAssignable, Category="VyraGameplayAbilit|GameplayTagStack")
	FOnTagStackChanged OnTagStackChanged;
	
private:

	UPROPERTY()
	FGameplayTagStackContainer TagStackContainer;

	UPROPERTY()
	TArray<FGameplayAbilityTagStackData> GameplayTagStack;
};
