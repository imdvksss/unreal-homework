// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyQuest : ModuleRules
{
	public MyQuest(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MyQuest",
			"MyQuest/Variant_Platforming",
			"MyQuest/Variant_Platforming/Animation",
			"MyQuest/Variant_Combat",
			"MyQuest/Variant_Combat/AI",
			"MyQuest/Variant_Combat/Animation",
			"MyQuest/Variant_Combat/Gameplay",
			"MyQuest/Variant_Combat/Interfaces",
			"MyQuest/Variant_Combat/UI",
			"MyQuest/Variant_SideScrolling",
			"MyQuest/Variant_SideScrolling/AI",
			"MyQuest/Variant_SideScrolling/Gameplay",
			"MyQuest/Variant_SideScrolling/Interfaces",
			"MyQuest/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
