#include "Core/RitualReflexComponent.h"
#include "Mind/EmotionCognitionComponent.h" // For LinkedMind

URitualReflexComponent::URitualReflexComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Enable ticking for MindTime updates
}

void URitualReflexComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize MindFocusState
    MindFocusState.BreathAmplitude = 0.0f;
    MindFocusState.InternalWhisper = TEXT("Silence.");
    MindFocusState.FocusMemoryID = TEXT("");
    MindFocusState.EmotionOverlay = FLinearColor::Black;
    MindFocusState.SensoryEchoAnchor = FVector::ZeroVector;
}

void URitualReflexComponent::OnBeginSecondBreathRitual()
{
    // Anchoring breath, echo of presence
    FRitualReflexEvent ThresholdEvent;
    ThresholdEvent.ReflexType = TEXT("Breath");
    ThresholdEvent.Timestamp = FDateTime::UtcNow();
    ThresholdEvent.PhaseMarker = TEXT("Threshold");
    RitualEvents.Add(ThresholdEvent);

    UE_LOG(LogTemp, Log, TEXT("[Ritual] Second Breath Threshold initialized at %s"), *ThresholdEvent.Timestamp.ToString());

    // Update MindFocusState for this ritual
    MindFocusState.InternalWhisper = TEXT("Breath as temporal anchor. Invitation of presence.");
    MindFocusState.FocusMemoryID = TEXT("SecondBreathThreshold");
}

void URitualReflexComponent::UpdateBreathPhase(float BreathAmplitude)
{
    // Update MindTime for internal oscillations
    MindTime += GetWorld()->GetDeltaSeconds();
    MindFocusState.BreathAmplitude = FMath::Clamp(0.58f + FMath::Sin(MindTime * 0.4f) * 0.07f, 0.0f, 1.0f); // Example oscillation

    for (FRitualReflexEvent& Event : RitualEvents)
    {
        if (Event.ReflexType == TEXT("Breath"))
        {
            float ModulationFactor = FMath::Clamp(BreathAmplitude, 0.0f, 1.0f);
            // Modulate nearby memory resonance
            if (LinkedMind)
            {
                // Accessing EmotionalMemoryBank directly requires it to be public or a getter to be available.
                // Assuming for now, a conceptual direct influence.
                for (FCognitiveMemoryNode& Memory : LinkedMind->EmotionalMemoryBank) // Assumes EmotionalMemoryBank is accessible
                {
                    Memory.EmotionalCharge = FMath::Lerp(Memory.EmotionalCharge, Memory.EmotionalCharge * (0.98f + 0.02f * ModulationFactor), GetWorld()->GetDeltaSeconds());
                    Memory.DecayProgress = FMath::Lerp(Memory.DecayProgress, Memory.DecayProgress * (0.99f - 0.01f * ModulationFactor), GetWorld()->GetDeltaSeconds());
                }
            }
            UE_LOG(LogTemp, Verbose, TEXT("[Ritual] Breath phase updated. Amplitude: %.2f. Modulating memory."), BreathAmplitude);

            // Update internal whisper based on breath amplitude
            if (ModulationFactor > 0.8f) MindFocusState.InternalWhisper = TEXT("Breath suspends time. Recognition of current emotional state.");
            else if (ModulationFactor < 0.2f) MindFocusState.InternalWhisper = TEXT("Fold settles. Memory imprint stabilizes.");
        }
    }
}
