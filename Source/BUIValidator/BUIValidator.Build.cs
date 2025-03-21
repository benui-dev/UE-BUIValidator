// Copyright ben ui. All Rights Reserved.

using UnrealBuildTool;

public class BUIValidator : ModuleRules
{
	public BUIValidator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});


		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"DataValidation",
			"UnrealEd"
		});
	}
}