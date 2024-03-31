// Copyright Epic Games, Inc. All Rights Reserved.


namespace UnrealBuildTool.Rules
{
	public class RemWidgetComponent : ModuleRules
	{
		public RemWidgetComponent(ReadOnlyTargetRules target) : base(target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			ShadowVariableWarningLevel = WarningLevel.Error;
			IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
			DefaultBuildSettings = BuildSettingsVersion.Latest;
			CppStandard = CppStandardVersion.EngineDefault;
			UnsafeTypeCastWarningLevel = WarningLevel.Warning;

			bEnableNonInlinedGenCppWarnings = true;
			bUseUnity = false;

			PrivateDependencyModuleNames.AddRange(
				new[]
				{
					"Core",
					"CoreUObject",
					"Engine",

					"UMG",
					"StructUtils",
					
					"RemCommon",
				}
			);
		}
	}
}
