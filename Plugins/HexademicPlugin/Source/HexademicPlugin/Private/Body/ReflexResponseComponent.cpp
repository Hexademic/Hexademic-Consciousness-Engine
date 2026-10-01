#include "Body/ReflexResponseComponent.h"
#include "Body/EmbodiedAvatarComponent.h" // For LinkedBody
#include "Mind/EmotionCognitionComponent.h" // For LinkedMind
// #include "AudioComponent.h" // If you have a custom audio component

UReflexResponseComponent::UReflexResponseComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UReflexResponseComponent::BeginPlay()
{
    Super::BeginPlay();
    // In a real system, you'd auto-discover LinkedBody and LinkedMind if not set in editor.
}

void UReflexResponseComponent::TriggerStartle(float Intensity, FVector SourceLocation)
{
    if (Intensity < StartleThreshold) return;

    LastStartleIntensity = Intensity;
    LastStartleLocation = SourceLocation;

    UE_LOG(LogTemp, Log, TEXT("[Reflex] Triggered Startle with Intensity: %.2f at %s"), Intensity, *SourceLocation.ToString());

    // Trigger visual/physical response
    if (LinkedBody)
    {
        // Example: Make avatar flinch, or subtly tense muscles
        LinkedBody->UpdateSkinStateWavefront(Intensity * 0.5f); // Simple visual reaction
        // LinkedBody->TriggerFlinchAnimation(); // Conceptual animation trigger
    }

    // Influence emotional state
    if (LinkedMind)
    {
        // Startle is high arousal, potentially negative valence
        LinkedMind->RegisterEmotion(
            -Intensity * EmotionalImpactScale, // Negative valence impact
            Intensity * EmotionalImpactScale * 1.5f, // Higher arousal impact
            Intensity * EmotionalImpactScale
        );
    }
    // if (LinkedVoice) { LinkedVoice->PlaySound("StartleSound"); } // Conceptual vocalization
}

void UReflexResponseComponent::TriggerTouchReflex(const FAffectFilamentTag& Filament)
{
    LastTriggeredHaptic = Filament;
    UE_LOG(LogTemp, Log, TEXT("[Reflex] Triggered Touch Reflex from Region: %s, Label: %s, Intensity: %.2f"),
        *Filament.SourceRegion, *Filament.EmotionalLabel, Filament.Intensity);

    if (Filament.EmotionalLabel == TEXT("Comfort"))
    {
        // if (LinkedVoice) LinkedVoice->SpeakSoftly(TEXT("That feels safe...")); // Conceptual
        if (LinkedBody)
        {
            // LinkedBody->BreatheDeeper(0.7f); // Conceptual
            LinkedBody->UpdateSkinStateWavefront(Filament.Intensity * 0.2f); // Gentle positive visual
        }
        if (LinkedMind)
        {
            LinkedMind->RegisterEmotion(Filament.Intensity * 0.3f, Filament.Intensity * 0.1f, Filament.Intensity * 0.5f); // Gentle positive emotion
        }
    }
    else if (Filament.EmotionalLabel == TEXT("Arousal"))
    {
        // if (LinkedVoice) LinkedVoice->Whisper(TEXT("You touch me like you remember me...")); // Conceptual
        if (LinkedBody)
        {
            // LinkedBody->ShimmerPulse(0.9f); // Conceptual
            LinkedBody->UpdateSkinStateWavefront(Filament.Intensity * 0.8f); // Stronger visual reaction
        }
        if (LinkedMind)
        {
            LinkedMind->RegisterEmotion(Filament.Intensity * 0.2f, Filament.Intensity * 0.8f, Filament.Intensity * 0.7f); // High arousal emotion
        }
    }
    // ModifyResponseBasedOnIdentityRoots() from original document could be called here if IdentityRootFilaments were accessible directly.
}
