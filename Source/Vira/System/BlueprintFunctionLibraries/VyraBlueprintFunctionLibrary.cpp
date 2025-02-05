﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraBlueprintFunctionLibrary.h"

#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Vira/AbilitySystem/Abilities/Player/Charms/VyraGameplayAbility_CharmBase.h"
#include "Vira/Player/VyraPlayerController.h"
#include "Vira/System/VyraGameInstance.h"

AVyraPlayerStateCharacter* UVyraBlueprintFunctionLibrary::GetVyraPlayerCharacter(UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AVyraPlayerStateCharacter* VyraCharacter = Cast<AVyraPlayerStateCharacter>(PC->GetPawn()))
		{
			return VyraCharacter;
		}
	}
	return nullptr;
}

AVyraPlayerController* UVyraBlueprintFunctionLibrary::GetVyraPlayerController(UObject* WorldContextObject)
{
	// Use UGameplayStatics to get the player controller from the world context object
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AVyraPlayerController* VyraPlayerController = Cast<AVyraPlayerController>(PlayerController))
		{
			return VyraPlayerController;
		}
	}

	// Return nullptr if no player controller was found
	return nullptr;
}

UVyraGameInstance* UVyraBlueprintFunctionLibrary::GetVyraGameInstance(UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	if (UVyraGameInstance* VGI = Cast<UVyraGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		return VGI;
	}
	
	return nullptr;
}

UVyraAbilitySystemComponent* UVyraBlueprintFunctionLibrary::GetVyraAbilitySystemComponent(UObject* WorldContextObject)
{
	if (AVyraPlayerStateCharacter* PlayerStateCharacter = GetVyraPlayerCharacter(WorldContextObject))
	{
		if (UVyraAbilitySystemComponent* ASC = Cast<UVyraAbilitySystemComponent>(PlayerStateCharacter->GetAbilitySystemComponent()))
		{
			return ASC;
		}
	}
	return nullptr;
}

UGSCCoreComponent* UVyraBlueprintFunctionLibrary::GetVyraPlayerGSCCoreComponent(UObject* WorldContextObject)
{
	if (AVyraPlayerStateCharacter* VyraCharacter = GetVyraPlayerCharacter(WorldContextObject))
	{
		return VyraCharacter->GetGSCCoreComponent();
	}
	return nullptr;
}

UVyraSaveGame_Charms* UVyraBlueprintFunctionLibrary::GetVyraPlayerSaveGameCharms(UObject* WorldContextObject)
{
	if (AVyraPlayerStateCharacter* VyraCharacter = GetVyraPlayerCharacter(WorldContextObject))
	{
		return VyraCharacter->GetSaveGameCharms();
	}
	return nullptr;
}

UCharmManagerComponent* UVyraBlueprintFunctionLibrary::GetVyraPlayerCharmManagerComponent(UObject* WorldContextObject)
{
	if (AVyraPlayerStateCharacter* VyraCharacter = GetVyraPlayerCharacter(WorldContextObject))
	{
		return VyraCharacter->GetCharmManagerComponent();
	}
	return nullptr;
}

TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> UVyraBlueprintFunctionLibrary::GetAllCharmAbilities(
	UObject* WorldContextObject)
{
	TArray<TSubclassOf<UVyraGameplayAbility_CharmBase>> FoundAbilities;

	// Ensure the Asset Manager is valid
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!AssetManager)
	{
		UE_LOG(LogTemp, Error, TEXT("AssetManager is invalid!"));
		return FoundAbilities;
	}

	// Specify the primary asset type for charm abilities
	FPrimaryAssetType CharmAbilityType = TEXT("GameplayAbility"); // Define this type in your asset manager settings if not already present

	// Get the primary asset list for this type
	TArray<FPrimaryAssetId> AssetIds;
	AssetManager->GetPrimaryAssetIdList(CharmAbilityType, AssetIds);

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		// Convert the AssetId to a SoftObjectPath
		FSoftObjectPath SoftObjectPath = UAssetManager::Get().GetPrimaryAssetPath(AssetId);

		// Ensure the SoftObjectPath is valid
		if (SoftObjectPath.IsValid())
		{
			// Attempt to load the class synchronously
			if (TSubclassOf<UVyraGameplayAbility_CharmBase> LoadedClass = Cast<UClass>(SoftObjectPath.TryLoad()))
			{
				FoundAbilities.Add(LoadedClass);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to load ability class for AssetId: %s"), *AssetId.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid SoftObjectPath for AssetId: %s"), *AssetId.ToString());
		}
	}

	return FoundAbilities;
}

TArray<FString> UVyraBlueprintFunctionLibrary::GetActiveGameInstanceSubsystemNames(const UObject* WorldContextObject)
{
	TArray<FString> SubsystemNames;

	if (!WorldContextObject) return SubsystemNames;

	if (const UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		// Use ForEachSubsystem to iterate through all GameInstanceSubsystems
		GameInstance->ForEachSubsystem<UGameInstanceSubsystem>([&SubsystemNames](const UGameInstanceSubsystem* Subsystem)
		{
			if (Subsystem)
			{
				SubsystemNames.Add(Subsystem->GetName());
			}
			return true; // Continue iteration
		});
	}

	return SubsystemNames;
}

void UVyraBlueprintFunctionLibrary::SaveCurrentLevel(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UVyraGameInstance* VyraGameInstance = Cast<UVyraGameInstance>(GameInstance))
		{
			VyraGameInstance->SaveCurrentLevel();
		}
	}
}

void UVyraBlueprintFunctionLibrary::LoadCurrentLevel(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UVyraGameInstance* VyraGameInstance = Cast<UVyraGameInstance>(GameInstance))
		{
			VyraGameInstance->LoadCurrentLevel();
		}
	}
}

void UVyraBlueprintFunctionLibrary::SavePlayerData(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UVyraGameInstance* VyraGameInstance = Cast<UVyraGameInstance>(GameInstance))
		{
			VyraGameInstance->SaveCurrentPlayerData();
		}
	}
}

void UVyraBlueprintFunctionLibrary::LoadPlayerData(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	if (UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance())
	{
		if (UVyraGameInstance* VyraGameInstance = Cast<UVyraGameInstance>(GameInstance))
		{
			VyraGameInstance->LoadCurrentPlayerData();
		}
	}
}

bool UVyraBlueprintFunctionLibrary::IsWithEditor()
{
	return WITH_EDITOR;
}

FString UVyraBlueprintFunctionLibrary::GetProjectVersion(const UObject* WorldContextObject)
{
	FString AppVersion;
	GConfig->GetString(
	  TEXT("/Script/EngineSettings.GeneralProjectSettings"),
	  TEXT("ProjectVersion"),
	  AppVersion,
	  GGameIni
	);
	return AppVersion;
}
