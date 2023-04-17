// Copyright Epic Games, Inc. All Rights Reserved.


namespace UnrealBuildTool.Rules
{
	public class RemWidgetComponent : ModuleRules
	{
		public RemWidgetComponent(ReadOnlyTargetRules target) : base(target)
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
					
					"RemCommon",
				}
			);
		}
	}
}
