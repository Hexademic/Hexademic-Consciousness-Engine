#include "SkinToneFluxShader.h"

USkinToneFluxShader::USkinToneFluxShader()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void USkinToneFluxShader::BeginPlay()
{
    Super::BeginPlay();
}
void USkinToneFluxShader::UpdateSkinVisuals(float EmotionalPulse, float SigilStrength, FLinearColor SigilColor)
{
    UE_LOG(LogTemp, Log, TEXT("[SkinToneFluxShader] Updating skin visuals: Pulse=%.2f, SigilStrength=%.2f, Color=%s"), EmotionalPulse, SigilStrength, *SigilColor.ToString());
    // This component would internally manage the compute shader dispatch for the skin.
    // In our case, EmbodiedAvatarComponent already does this, so this is just a log.
}
