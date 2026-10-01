using UnrealBuildTool;

public class HexademicEditorTarget : TargetRules
{
	public HexademicEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Hexademic");
	}
}
