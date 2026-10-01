using UnrealBuildTool;
using System.IO;

public class HexademicPlugin : ModuleRules
{
	public HexademicPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 2026-10-01: set explicitly for UE 5.7. The original file predated 5.4 and
		// declared neither, so it inherited whatever defaults the engine of the day
		// supplied — one more thing that could differ between machines without saying so.
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Niagara",        // sigil / bloom VFX
			"RHI",            // render graph + global shaders
			"RenderCore",
			"Renderer",
			"Projects",       // IPluginManager, for the virtual shader path mapping below
			"Json",
			"JsonUtilities",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// 2026-10-01 — REMOVED, and this is why nothing ever compiled.
			//
			// This list declared nine modules: HexademicCore, HexademicMind,
			// HexademicBody, HexademicBridge, HexademicAPI, PhenomenalConsciousness,
			// HexademicVisuals, HexademicLiving and HexademicFractal. NOT ONE OF THEM
			// EXISTS — there is no `.Build.cs` for any of them anywhere in the tree.
			// Each carried a hedging comment ("If HexademicCore is a separate module"),
			// so they were written speculatively and never resolved.
			//
			// UnrealBuildTool fails on the first unresolvable module name, before it
			// reads a single line of C++. So in nine and a half months and 109 source
			// files, this project never once produced a compiler error — not a passing
			// build, not a failing one. No feedback of any kind.
			//
			// Those nine names are directories inside Public/, not modules. They are
			// covered by the include paths below.
			"AIModule",       // HexademicAIController / AIPawn / Blackboard
			"GameplayTasks",  // required alongside AIModule
			"Slate",
			"SlateCore",
		});

		// 2026-10-01 — CORRECTED. Every one of these pointed at `ModuleDirectory/<Name>`,
		// but the sources live under `ModuleDirectory/Public/<Name>`. All eleven were
		// wrong by exactly one directory level, which would have been the second failure
		// had the first been fixed.
		string Pub = Path.Combine(ModuleDirectory, "Public");
		foreach (string Dir in new string[]
		{
			"Mind", "Body", "Visuals", "Living", "PhenomCollective",
			"Intersubjective", "Fractal", "Bridge", "API", "Core", "Components",
			"Subsystems", "AI",
		})
		{
			string Candidate = Path.Combine(Pub, Dir);
			if (Directory.Exists(Candidate))
			{
				PrivateIncludePaths.Add(Candidate);
			}
		}
	}
}
