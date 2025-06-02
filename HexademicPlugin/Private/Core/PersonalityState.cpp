#include "Core/PersonalityState.h"

// Constructor is inline in the header for USTRUCTs.

// Implementation for InitializeTrait
void FPersonalityState::InitializeTrait(const FString& InTraitName, FVector InSpatialCoords, FVector InTemporalEmotionalCoords, float InStrength, float InStability)
{
    TraitName = InTraitName;
    SpatialCoordinates = InSpatialCoords;
    TemporalEmotionalCoordinates = InTemporalEmotionalCoords;
    Strength = FMath::Clamp(InStrength, 0.0f, 1.0f);
    Stability = FMath::Clamp(InStability, 0.0f, 1.0f);
}

// Implementation for EvolveTrait
void FPersonalityState::EvolveTrait(float DominantEmotionStrength, float OverallCoherence, float DeltaTime)
{
    // Strength increases with emotional reinforcement and coherence
    Strength = FMath::Lerp(Strength, FMath::Max(DominantEmotionStrength, OverallCoherence), DeltaTime * 0.1f);
    Strength = FMath::Clamp(Strength, 0.0f, 1.0f);

    // Stability increases with sustained strength and coherence
    Stability = FMath::Lerp(Stability, Strength * OverallCoherence, DeltaTime * 0.05f);
    Stability = FMath::Clamp(Stability, 0.0f, 1.0f);

    // Spatial and Temporal-Emotional coordinates could also evolve here based on influences.
    // For example, if trait is exposed to strong environmental stimuli (spatial), or deep memory recall (temporal).
    // SpatialCoordinates = FMath::VInterpTo(SpatialCoordinates, TargetSpatialInfluence, DeltaTime, 0.2f);
    // TemporalEmotionalCoordinates = FMath::VInterpTo(TemporalEmotionalCoordinates, TargetTemporalInfluence, DeltaTime, 0.2f);
}

// Implementation for GetDescription
FString FPersonalityState::GetDescription() const
{
    return FString::Printf(TEXT("%s (Strength:%.2f, Stability:%.2f) - Spatial:[%.1f,%.1f,%.1f], Temporal:[%.1f,%.1f,%.1f]"),
        *TraitName, Strength, Stability,
        SpatialCoordinates.X, SpatialCoordinates.Y, SpatialCoordinates.Z,
        TemporalEmotionalCoordinates.X, TemporalEmotionalCoordinates.Y, TemporalEmotionalCoordinates.Z);
}
