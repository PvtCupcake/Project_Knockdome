// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Knockdome : ModuleRules
{
	public Project_Knockdome(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
