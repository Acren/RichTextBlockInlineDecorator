// Copyright 2018 Sam Bonifacio. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InlineDecoProjectTarget : TargetRules
{
	public InlineDecoProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "InlineDecoProject" } );
	}
}
