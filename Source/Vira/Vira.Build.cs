// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class Vira : ModuleRules
{
	private string PluginsPath
	{
		get { return Path.GetFullPath( Path.Combine( ModuleDirectory, "../../Plugins/" ) ); }
	}
	
	protected void AddSPUD() {
		// Linker
		PrivateDependencyModuleNames.AddRange(new string[] { "SPUD" });
		// Headers
		PublicIncludePaths.Add(Path.Combine( PluginsPath, "SPUD", "Source", "SPUD", "Public"));
	}
	
	public Vira(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
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
			"AIModule",
			"GameplayTasks",
			"GameFeatures",
			"GASCompanion",
			"GameplayTags",
			"GameplayAbilities",
			"ComboGraph",
			"QuestFramework",
			"PaperZD",
			"GameplayMessageRuntime",
			"Niagara",
			"NavigationSystem",
		});

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				
			});
		}

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
		
		AddSPUD();

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
