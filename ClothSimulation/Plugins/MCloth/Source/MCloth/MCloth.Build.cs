using UnrealBuildTool;

public class MCloth : ModuleRules

{

    public MCloth(ReadOnlyTargetRules Target) : base(Target)

    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(new string[]
        {
            "MCloth/Private"
        });
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("TargetPlatform");
        }
        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("MaterialShaderQualitySettings");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Renderer",
            "RenderCore",
            "RHI",
            "Projects"
        });

        if (Target.bBuildEditor == true)
        {

            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UnrealEd",
                    "MaterialUtilities",
                    "SlateCore",
                    "Slate"
                }
            );
        }
    }

}