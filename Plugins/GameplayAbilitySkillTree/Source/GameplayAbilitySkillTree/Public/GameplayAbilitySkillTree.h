// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FGameplayAbilitySkillTreeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FSlateStyleSet> StyleSetStyle = nullptr;
	TSharedPtr<struct FSkillTreeCustomPinFactory> PinFactory = nullptr;
};
