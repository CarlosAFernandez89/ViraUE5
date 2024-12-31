// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;

public class Vira : ModuleRules
{
	public Vira(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"CommonInput",
				"Slate",
				"SlateCore",
				"UMG",
				"CommonUI",
				"GameplayTasks",
				"GameFeatures",
				"GASCompanion",
				"GameplayTags",
				"GameplayAbilities",
				"QuestFramework",
				"PaperZD",
				"GameplayMessageRuntime",
			});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Paper2D",
			"GameplayTasks",
			"GameplayTags",
			"AIModule",
			"Slate",
			"SlateCore",
			"DeveloperSettings",
			"ModularGameplay",
			"NetCore",// For OSx
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
