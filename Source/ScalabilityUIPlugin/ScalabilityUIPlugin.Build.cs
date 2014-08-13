// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class ScalabilityUIPlugin : ModuleRules
	{
        public ScalabilityUIPlugin(TargetInfo Target)
		{
            PrivateIncludePaths.Add("ScalabilityUIPlugin/Private");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "Engine",
                    "InputCore",
                    "Slate",
				    "SlateCore"
				}
				);
		}
	}
}