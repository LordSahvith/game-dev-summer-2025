// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPS_Template1 : ModuleRules
{
	public FPS_Template1(ReadOnlyTargetRules Target) : base(Target)
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
			"FPS_Template1",
			"FPS_Template1/Variant_Horror",
			"FPS_Template1/Variant_Horror/UI",
			"FPS_Template1/Variant_Shooter",
			"FPS_Template1/Variant_Shooter/AI",
			"FPS_Template1/Variant_Shooter/UI",
			"FPS_Template1/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
