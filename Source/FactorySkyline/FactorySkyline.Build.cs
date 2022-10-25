using UnrealBuildTool;
using System.IO;
using System;

public class FactorySkyline : ModuleRules
{
    public FactorySkyline(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        ShadowVariableWarningLevel = WarningLevel.Off;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject",
            "Engine",
            "InputCore",
            "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNULL", "OnlineSubsystemEOS",
            "SignificanceManager",
            "PhysX", "APEX", "PhysXVehicles", "ApexDestruction",
            "AkAudio",
            "ReplicationGraph",
            "UMG",
            "AIModule",
            "NavigationSystem",
            "AssetRegistry",
            "GameplayTasks",
            "AnimGraphRuntime",
            "Slate", "SlateCore",
            "InstancedSplines"
            });

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "OnlineBlueprintSupport", "AnimGraph" });
        }

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Engine", "ApplicationCore" });
        //PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });


        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});
    }
}