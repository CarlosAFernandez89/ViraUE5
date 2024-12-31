// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CharmManagerComponent.generated.h"


struct FGameplayTag;
struct FSaveGame_Charms;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharmsUpdated, TSubclassOf<UVyraGameplayAbility_CharmBase>, CharmClass);

class AVyraPlayerStateCharacter;
class UVyraGameplayAbility_CharmBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UCharmManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharmManagerComponent();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> GetEquippedCharms() const {return EquippedCharmAbilities;}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> GetOwnedCharms() const {return OwnedCharmAbilities;}

	UFUNCTION(BlueprintCallable)
	class UVyraAbilitySystemComponent* GetVyraAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	class AVyraPlayerStateCharacter* GetVyraPlayerStateCharacter() const;

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	bool GiveCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> Ability_CharmBase);

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	bool RemoveCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> Ability_CharmBase);

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	bool EquipCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> CharmToEquip);

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	void UnEquipCharmAbility(const TSubclassOf<UVyraGameplayAbility_CharmBase> CharmToUnEquip);

	UFUNCTION()
	void SetCharmsOnLoad(const FSaveGame_Charms& AllCharms);

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	void AddStackTagToAbility(TSubclassOf<UVyraGameplayAbility_CharmBase> CharmAbilityType, FGameplayTag Tag, int32 StackCount);

	UFUNCTION(BlueprintCallable, Category="CharmManager")
	void RemoveStackTagFromAbility(TSubclassOf<UVyraGameplayAbility_CharmBase> CharmAbilityType, FGameplayTag Tag, int32 StackCount);
private:
	UPROPERTY()
	TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> EquippedCharmAbilities;
	
	UPROPERTY()
	TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> OwnedCharmAbilities;

public:
	UPROPERTY(BlueprintAssignable, Category="CharmManager")
	FOnCharmsUpdated OnCharmsUpdated;

};
