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
