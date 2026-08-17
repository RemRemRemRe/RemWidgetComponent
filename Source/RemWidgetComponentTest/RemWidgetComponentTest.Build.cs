// Copyright RemRemRemRe. 2026. All Rights Reserved.

using UnrealBuildTool;
using Rem.BuildRule;

public class RemWidgetComponentTest : ModuleRules
{
	public RemWidgetComponentTest(ReadOnlyTargetRules target) : base(target)
	{
		RemSharedModuleRules.Apply(this);

		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",

				"UMG",
				"SlateCore",

				"RemCommon",
				"RemWidgetComponent",
			]
		);
	}
}
