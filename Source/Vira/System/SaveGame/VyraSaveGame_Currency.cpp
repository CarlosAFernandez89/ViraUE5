// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraSaveGame_Currency.h"

UVyraSaveGame_Currency::UVyraSaveGame_Currency(): Currency_Souls(0)
{
	SaveSlotName = TEXT("VyraSaveGame_Currency");
	UserIndex = 0;
}

void UVyraSaveGame_Currency::SaveCurrency(const FSaveGame_Currency Currency)
{
	Currency_Souls = Currency.Souls;
}

FSaveGame_Currency UVyraSaveGame_Currency::LoadCurrency() const
{
	return FSaveGame_Currency(Currency_Souls);
}
