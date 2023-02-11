// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UE5Build : ModuleRules
{
	public UE5Build(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{  });

		PublicIncludePaths.AddRange(new string[]
        {
            "UE5Build/Public/Player",
			"UE5Build/Public/Components",
            "UE5Build/Public/Dev",
            "UE5Build/Public/Weapon",
            "UE5Build/Public/Weapon/Components",
            "UE5Build/Public/UI",
            "UE5Build/Public/Animations",
            "UE5Build/Public/Pickups",
            "UE5Build/Public/AI",
            "UE5Build/Public/AI/Tasks",
			"UE5Build/Public/AI/Services",
            "UE5Build/Public/AI/EQS",
            "UE5Build/Public/AI/Decorators"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
