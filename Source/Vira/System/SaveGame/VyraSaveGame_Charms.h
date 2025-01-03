// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Vira/AbilitySystem/Abilities/Player/Charms/VyraGameplayAbility_CharmBase.h"
#include "VyraSaveGame_Charms.generated.h"

USTRUCT(BlueprintType)
struct FSaveGame_Charms
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> EquippedCharms;
	UPROPERTY()
	TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> OwnedCharms;
};
/**
 * 
 */
UCLASS()
class VIRA_API UVyraSaveGame_Charms : public USaveGame
{
	GENERATED_BODY()

public:
	UVyraSaveGame_Charms();

	UFUNCTION()
	void SaveCharms(const UCharmManagerComponent* CharmManagerComponent);

	UFUNCTION()
	FSaveGame_Charms GetAllCharms() const;

	UFUNCTION()
	TMap<FName,FCharmAbilityData> GetAllCharmTagStacks() const;

	UFUNCTION()
	void UpdateCharmAbilityDataTMap(const FCharmAbilityData& AbilityData);

	UFUNCTION()
	void RemoveFromCharmAbilityDataTMap(const FCharmAbilityData& AbilityData);

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

private:
	UPROPERTY()
	FSaveGame_Charms AllCharms;
	
	UPROPERTY()
    TMap<FName,FCharmAbilityData> CharmAbilityData;
};
