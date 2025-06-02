#include "Core/QuantumAnalogState.h"

// Constructor: FQuantumAnalogState() is often defined in the header for structs, but can be here.
// FQuantumAnalogState::FQuantumAnalogState()
//     : Coherence(0.5f), EntangledSourceID(TEXT("None")), EntanglementStrength(0.0f),
//       ResonanceFrequency(1.0f), QuantumSignatureGlyph(TEXT("")), QuantumFlux(0.0f),
//       QuantumColor(FLinearColor::Black)
// {}

// Implementation for UpdateStateFromConsciousness
void FQuantumAnalogState::UpdateStateFromConsciousness(float InCoherence, float InEntanglementStrength, float InEmotionalIntensity, float DeltaTime)
{
    Coherence = FMath::Lerp(Coherence, InCoherence, DeltaTime * 0.5f);
    EntanglementStrength = FMath::Lerp(EntanglementStrength, InEntanglementStrength, DeltaTime * 0.5f);
    
    QuantumFlux = FMath::Lerp(QuantumFlux, InEmotionalIntensity * 0.2f, DeltaTime * 0.8f);

    ResonanceFrequency = FMath::Lerp(ResonanceFrequency, 1.0f + InCoherence * 2.0f, DeltaTime * 0.3f);

    QuantumColor = FLinearColor(Coherence, EntanglementStrength, InEmotionalIntensity, 1.0f);
}

// Implementation for IsHighlyCoherent
bool FQuantumAnalogState::IsHighlyCoherent() const
{
    return Coherence > 0.8f;
}
