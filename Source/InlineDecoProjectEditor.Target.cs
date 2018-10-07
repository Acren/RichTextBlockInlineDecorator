// Copyright 2018 Sam Bonifacio. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InlineDecoProjectEditorTarget : TargetRules
{
	public InlineDecoProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "InlineDecoProject" } );
	}
}
