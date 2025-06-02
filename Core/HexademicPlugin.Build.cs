// In HexademicPlugin.Build.cs
using UnrealBuildTool;
using System.IO; // Required for Path.Combine

public class HexademicPlugin : ModuleRules // Replace HexademicPlugin with your actual module name
{
    public HexademicPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs; // Or PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Niagara", // For Niagara effects
                "RHI", // For Render Graph and Shaders
                "RenderCore", // For Render Graph and Shaders
                "Renderer", // For Render Graph and Shaders
                "Json", // For JSON export
                "JsonUtilities", // For JSON export
                // Add any other modules for your dummy components if they need them
                // e.g., "AIModule", "GameplayTasks", etc.
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // Your internal modules/folders (if they are separate modules)
                "HexademicCore", // If HexademicCore is a separate module
                "HexademicMind", // If Mind components are in a separate module
                "HexademicBody", // If Body components are in a separate module
                "HexademicBridge", // If Bridge components are in a separate module
                "HexademicAPI", // If API components (DUIDSOrchestrator, WavefrontAPI) are in a separate module
                "PhenomenalConsciousness", // If PhenomCollective is a separate module
                "HexademicVisuals", // If Visuals components are in a separate module
                "HexademicLiving", // If Living components are in a separate module
                "HexademicFractal", // If Fractal components are in a separate module
            }
        );

        // Add PrivateIncludePaths if your .usf shader is not in the default project "Shaders" folder
        // For example, if it's in a subdirectory like "HexademicPlugin/Shaders"
        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Mind"),
                Path.Combine(ModuleDirectory, "Body"),
                Path.Combine(ModuleDirectory, "Visuals"),
                Path.Combine(ModuleDirectory, "Living"),
                Path.Combine(ModuleDirectory, "PhenomCollective"),
                Path.Combine(ModuleDirectory, "Intersubjective"), // For EmpathicFieldComponent
                Path.Combine(ModuleDirectory, "Fractal"), // For FractalConsciousnessManagerComponent
                Path.Combine(ModuleDirectory, "Shaders"), // Add your shader path
                Path.Combine(ModuleDirectory, "Bridge"), // For ConsciousnessBridgeComponent and AvatarMotionLinkComponent
                Path.Combine(ModuleDirectory, "API"), // For DUIDSOrchestrator and HexademicWavefrontAPI
                Path.Combine(ModuleDirectory, "Core"), // For HexademicCore and Glyph_AetherSkin
                // etc. for all your custom component folders
            }
        );

        // This is crucial for global shaders.
        // It tells Unreal where to find your .usf files.
        // It points to the root of your plugin's shader directory.
        // If your FWavefrontSkinComputeShader.usf is in "YourPlugin/Shaders/FWavefrontSkinComputeShader.usf"
        // then the mapping in IMPLEMENT_GLOBAL_SHADER should be "/YourPlugin/FWavefrontSkinComputeShader.usf"
        // And you need to ensure this is set up correctly in your build system.
        // PublicDefinitions.Add("WITH_HEXADEMIC_SHADERS=1"); // Example for conditional compilation if needed
    }
}
