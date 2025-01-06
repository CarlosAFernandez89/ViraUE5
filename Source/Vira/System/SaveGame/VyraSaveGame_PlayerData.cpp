// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraSaveGame_PlayerData.h"

#include "Components/QuestComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Vira/AbilitySystem/AttributeSets/CurrencyAttributeSet.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"

UVyraSaveGame_PlayerData::UVyraSaveGame_PlayerData(): Currency_Souls(0), Currency_Gold(0), TotalPlayTime(0)
{
	UserIndex = 0;
	SaveSlotName = "";
}

void UVyraSaveGame_PlayerData::SaveAllPlayerData(const FString& SlotName,UObject* WorldContextObject)
{
	SaveSlotName = SlotName;
	
	FSaveGamePlayerData_Currency Currency = GetCurrentPlayerCurrency(WorldContextObject);
	{
		Currency_Souls = Currency.Souls;
		Currency_Gold = Currency.Gold;
	}
	FQuestSaveStateData QuestSaveData = GetCurrentQuestState(WorldContextObject);
	{
		CurrentQuestSaveState = QuestSaveData;
	}

	if (UVyraGameInstance* GameInstance = UVyraBlueprintFunctionLibrary::GetVyraGameInstance(WorldContextObject))
	{
		TotalPlayTime = GameInstance->GetPlayTime();
	}


	FAsyncSaveGameToSlotDelegate SaveDelegate = FAsyncSaveGameToSlotDelegate::CreateLambda(
	[WorldContextObject](const FString& SavedSlotName, const int32 SavedUserIndex, bool bSuccess)
	{
	});
	
	UGameplayStatics::AsyncSaveGameToSlot(this, SaveSlotName, UserIndex, SaveDelegate);
}

void UVyraSaveGame_PlayerData::LoadAllPlayerData(UObject* WorldContextObject)
{
	if (UVyraAbilitySystemComponent* ASC = UVyraBlueprintFunctionLibrary::GetVyraAbilitySystemComponent(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		if (ASC->GetAttributeSet(UCurrencyAttributeSet::StaticClass()))
		{
			ASC->SetNumericAttributeBase(UCurrencyAttributeSet::GetSoulsAttribute(), Currency_Souls);
			//ASC->SetNumericAttributeBase(UCurrencyAttributeSet::GetGoldAttribute(), PlayerData->Currency_Gold);
		}
	}

	if (AVyraPlayerController* PC = UVyraBlueprintFunctionLibrary::GetVyraPlayerController(WorldContextObject))
	{
		if (UQuestComponent* QuestComponent =PC->GetComponentByClass<UQuestComponent>())
		{
			QuestComponent->LoadQuestStates(CurrentQuestSaveState, true);
		}
	}
}

FSaveGamePlayerData_Currency UVyraSaveGame_PlayerData::GetCurrentPlayerCurrency(UObject* WorldContextObject)
{
	if (UVyraAbilitySystemComponent* ASC = UVyraBlueprintFunctionLibrary::GetVyraAbilitySystemComponent(WorldContextObject))
	{
		FSaveGamePlayerData_Currency Temp;
		if (ASC->GetAttributeSet(UCurrencyAttributeSet::StaticClass()))
		{
			Temp.Souls = ASC->GetNumericAttribute(UCurrencyAttributeSet::GetSoulsAttribute());
			//Temp.Gold = ASC->GetNumericAttribute(UCurrencyAttributeSet::GetGoldAttribute());
		}
		return Temp;
	}

	return FSaveGamePlayerData_Currency();
}

FQuestSaveStateData UVyraSaveGame_PlayerData::GetCurrentQuestState(UObject* WorldContextObject)
{
	if (AVyraPlayerController* PC = UVyraBlueprintFunctionLibrary::GetVyraPlayerController(WorldContextObject))
	{
		if (UQuestComponent* QuestComponent =PC->GetComponentByClass<UQuestComponent>())
		{
			FQuestSaveStateData TempSaveData;
			QuestComponent->SaveQuestStates(TempSaveData);
			return TempSaveData;
		}
	}
	return FQuestSaveStateData();
}

void UVyraSaveGame_PlayerData::UpdateCurrency(const FSaveGamePlayerData_Currency& Data)
{
	// Add all currency values here, make sure to update the Struct.
	Currency_Souls = Data.Souls;
	Currency_Gold = Data.Gold;
}

FSaveGamePlayerData_Currency UVyraSaveGame_PlayerData::GetLoadedCurrency()
{
	return FSaveGamePlayerData_Currency(Currency_Souls, Currency_Gold);
}


