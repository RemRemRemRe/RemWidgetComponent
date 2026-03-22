// Copyright RemRemRemRe. All Rights Reserved.

using UnrealBuildTool;
using Rem.BuildRule;

public class RemWidgetComponent : ModuleRules
{
	public RemWidgetComponent(ReadOnlyTargetRules target) : base(target)
	{
        RemSharedModuleRules.Apply(this);
		
		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",
				
				"UMG",
				
				"RemCommon",
			]
		);
	}
}
