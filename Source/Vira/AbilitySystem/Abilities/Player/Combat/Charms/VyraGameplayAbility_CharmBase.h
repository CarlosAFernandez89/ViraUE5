// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/AbilitySystem/Abilities/VyraGameplayAbility.h"
#include "Vira/System/GameplayTagStack.h"
#include "Engine/Texture.h"

#include "VyraGameplayAbility_CharmBase.generated.h"

USTRUCT(BlueprintType)
struct FCharmAbilityData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GameplayTagStackTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameplayTagStackTagCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameplayTagStackTagMaxCount = -1;

};

USTRUCT(BlueprintType)
struct FCharmAbilityInformation
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* Icon;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTagStacksChanged);

/**
 * 
 */
UCLASS()
class VIRA_API UVyraGameplayAbility_CharmBase : public UVyraGameplayAbility
{
	GENERATED_BODY()

public:
	UVyraGameplayAbility_CharmBase();

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GameplayAbility|Charms")
	void SetStatTagStack(FGameplayTag Tag, const int32 StackCount);
	
	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GameplayAbility|Charms")
	void AddStatTagStack(FGameplayTag Tag, const int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GameplayAbility|Charms")
	void RemoveStatTagStack(FGameplayTag Tag, const int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category="GameplayAbility|Charms")
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category="GameplayAbility|Charms")
	int32 GetMaxStatTagStackCount(FGameplayTag Tag);

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category="GameplayAbility|Charms")
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable,Category="GameplayAbility|Charms")
	const FGameplayTagStackContainer& GetTagStackContainer() const
	{
		return TagStackContainer;
	}

	void SetStatTags(const FGameplayTagStackContainer& InTagStackContainer)
	{
		TagStackContainer = InTagStackContainer;
	}

	int32 FindCharmAbilityDataIndexByGameplayTag(const FGameplayTag& TagToFind) const;

	int32 ClampToMaxStackCount(FGameplayTag Tag, int32 StackCount);

	int32 ClampToMinStackCount(FGameplayTag Tag, int32 StackCount);


protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintNativeEvent, Category="GameplayAbility|Charms")
	void OnTagStacksChangedEvent();

	UFUNCTION(BlueprintImplementableEvent, Category="GameplayAbility|Charms")
	void PostInitStatTagStack();
	
private:

	UPROPERTY()
	FGameplayTagStackContainer TagStackContainer;


protected:

	UPROPERTY(BlueprintAssignable)
	FOnTagStacksChanged OnTagStacksChanged;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharmAbilityData> CharmAbilityData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharmAbilityInformation CharmAbilityInformation;
	
};
