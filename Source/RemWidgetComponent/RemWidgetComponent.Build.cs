// Copyright RemRemRemRe. All Rights Reserved.

using UnrealBuildTool;

public class RemWidgetComponent : ModuleRules
{
	public RemWidgetComponent(ReadOnlyTargetRules target) : base(target)
	{
		ShadowVariableWarningLevel = WarningLevel.Error;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppStandard = CppStandardVersion.EngineDefault;
		UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		
		bEnableNonInlinedGenCppWarnings = true;
		bUseUnity = false;
		
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
