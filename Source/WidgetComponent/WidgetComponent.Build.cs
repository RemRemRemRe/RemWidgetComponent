// Copyright Epic Games, Inc. All Rights Reserved.


namespace UnrealBuildTool.Rules
{
	public class WidgetComponent : ModuleRules
	{
		public WidgetComponent(ReadOnlyTargetRules target) : base(target)
		{
			PCHUsage					= PCHUsageMode.UseExplicitOrSharedPCHs;
			bLegacyPublicIncludePaths	= false;
			ShadowVariableWarningLevel	= WarningLevel.Error;
			CppStandard 				= CppStandardVersion.Cpp20;
			
			PrivateDependencyModuleNames.AddRange(
				new[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"UMG",
					"Common"
				}
			);
		}
	}
}
