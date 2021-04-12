// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VertiScape : ModuleRules
{
	public VertiScape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "HeadMountedDisplay", "NavigationSystem" });
	}
}
