// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TrianglesTechDemo : ModuleRules
{
	public TrianglesTechDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
