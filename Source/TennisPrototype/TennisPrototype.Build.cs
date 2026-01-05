// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TennisPrototype : ModuleRules
{
	public TennisPrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TennisPrototype",
			"TennisPrototype/Variant_Platforming",
			"TennisPrototype/Variant_Combat",
			"TennisPrototype/Variant_Combat/AI",
			"TennisPrototype/Variant_SideScrolling",
			"TennisPrototype/Variant_SideScrolling/Gameplay",
			"TennisPrototype/Variant_SideScrolling/AI",
			"TennisPrototype/Tennis_Prototype"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
