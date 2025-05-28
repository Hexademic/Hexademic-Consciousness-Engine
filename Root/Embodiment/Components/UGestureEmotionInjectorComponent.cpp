#include "UGestureEmotionInjectorComponent.h"
#include "HexademicEmotionManagerSubsystem.h"

UGestureEmotionInjectorComponent::UGestureEmotionInjectorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    GestureEmotionMap.Add("OpenPalmForward", ECoreEmotion::Curiosity);
    GestureEmotionMap.Add("ClenchedFist",     ECoreEmotion::Rage);
    GestureEmotionMap.Add("LoweredHead",      ECoreEmotion::Grief);
    GestureEmotionMap.Add("RaisedArms",       ECoreEmotion::Awe);
    GestureEmotionMap.Add("RetreatStep",      ECoreEmotion::Fear);
    GestureEmotionMap.Add("HandToChest",      ECoreEmotion::Longing);
    GestureEmotionMap.Add("JoyLeap",          ECoreEmotion::Joy);
}

void UGestureEmotionInjectorComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UGestureEmotionInjectorComponent::InjectGestureEmotion(FName GestureTag, float Intensity)
{
    if (!GestureEmotionMap.Contains(GestureTag)) return;
    MapGestureToEmotion(GestureTag, Intensity);
}

void UGestureEmotionInjectorComponent::MapGestureToEmotion(FName GestureTag, float Intensity)
{
    ECoreEmotion MappedEmotion = GestureEmotionMap[GestureTag];

    // This assumes a global emotion manager exists — replace with your lattice
    if (UHexademicEmotionManagerSubsystem* EmotionSystem = GetWorld()->GetSubsystem<UHexademicEmotionManagerSubsystem>())
    {
        EmotionSystem->InjectCoreEmotion(MappedEmotion, Intensity * GlobalSensitivity);
    }
}
