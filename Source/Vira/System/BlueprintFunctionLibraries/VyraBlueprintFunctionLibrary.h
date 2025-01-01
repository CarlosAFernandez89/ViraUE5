// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "VyraBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VIRA_API UVyraBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static AVyraPlayerStateCharacter* GetVyraPlayerCharacter(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static UPaperFlipbookComponent* GetVyraPlayerSprite(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static UPaperZDAnimInstance* GetVyraPlayerPaperZDAnimInstance(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static UGSCCoreComponent* GetVyraPlayerGSCCoreComponent(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static UVyraSaveGame_Charms* GetVyraPlayerSaveGameCharms(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Character", meta = (WorldContext = "WorldContextObject"))
	static UCharmManagerComponent* GetVyraPlayerCharmManagerComponent(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Vyra|Assets", meta = (WorldContext = "WorldContextObject"))
	static TArray<TSubclassOf<class UVyraGameplayAbility_CharmBase>> GetAllCharmAbilities(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Subsystem")
	static TArray<FString> GetActiveGameInstanceSubsystemNames(const UObject* WorldContextObject);
};
