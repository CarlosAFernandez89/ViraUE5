// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Vira/System/GameplayTagStack.h"
#include "GameplayTagStackComponent.generated.h"

USTRUCT(BlueprintType)
struct FGameplayAbilityTagStackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GameplayTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCount = -1;
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
	void SetTagStack(FGameplayTag Tag, const int32 NewStackCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void AddToTagStack(FGameplayTag Tag, const int32 CountToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="VyraGameplayAbilit|GameplayTagStack")
	void RemoveTagStack(FGameplayTag Tag, const int32 CountToRemove);

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
	int32 ClampToMaxStackCount(FGameplayTag Tag, const int32 MaxCount) const;
	int32 ClampToMinStackCount(FGameplayTag Tag, const int32 MinCount) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayAbilityTagStackData> GameplayTagStack;
	
	UPROPERTY(BlueprintAssignable, Category="VyraGameplayAbilit|GameplayTagStack")
	FOnTagStackChanged OnTagStackChanged;
	
private:

	UPROPERTY()
	FGameplayTagStackContainer TagStackContainer;
};
