// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraStringUtils.h"

FString UVyraStringUtils::StringArrayToString(UObject* WorldContextObject, TArray<FString> StringArray, FString StartingString)
{
	if (!WorldContextObject) return FString();
	
	FString ReturnString = StartingString;

	for (int i = 0; i < StringArray.Num(); i++)
	{
		ReturnString.Append(StringArray[i] + "\n");
	}
	return ReturnString;
}

FString UVyraStringUtils::DamageToShortStringFormater(UObject* WorldContextObject, float InFloat, bool bIncludeDecimals, int32 InDecimals)
{
	if (!WorldContextObject) return FString();

	const TArray<TTuple<double, FString>> Suffixes = {
		{1e18, "Qi"}, // Quintillion
		{1e15, "Qa"}, // Quadrillion
		{1e12, "T"},  // Trillion
		{1e9,  "B"},  // Billion
		{1e6,  "M"},  // Million
		{1e3,  "k"}   // Thousand
	};

	const double AbsNumber = FMath::Abs(InFloat);
	FString Suffix = "";
	double Divisor = 1.0;

	// Find appropriate suffix
	for (const auto& Pair : Suffixes)
	{
		if (AbsNumber >= Pair.Get<0>())
		{
			Divisor = Pair.Get<0>();
			Suffix = Pair.Get<1>();
			break;
		}
	}

	const double Value = InFloat / Divisor;

	// Configure number formatting
	FNumberFormattingOptions FormatOptions;
	FormatOptions.UseGrouping = true;
	FormatOptions.RoundingMode = ERoundingMode::HalfFromZero;
    
	if (bIncludeDecimals)
	{
		FormatOptions.MinimumFractionalDigits = 0;
		FormatOptions.MaximumFractionalDigits = (FMath::Abs(Value - FMath::FloorToDouble(Value)) > 0) ? InDecimals : 0;
	}
	else
	{
		FormatOptions.MinimumFractionalDigits = 0;
		FormatOptions.MaximumFractionalDigits = 0;
	}

	// Format the number
	FString FormattedNumber = FText::AsNumber(Value, &FormatOptions).ToString();
    
	// Remove trailing .0 if no decimals
	if (FormattedNumber.EndsWith(".0"))
	{
		FormattedNumber.LeftChopInline(2);
	}

	return FormattedNumber + Suffix;
}

FString UVyraStringUtils::DamageToLongStringFormater(UObject* WorldContextObject, float InFloat)
{
	if (!WorldContextObject) return FString();
	
	return FString::FormatAsNumber(FMath::RoundToInt(InFloat));
}
