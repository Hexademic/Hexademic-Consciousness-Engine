#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h" // Needed for USTRUCT
#include "Math/Vector.h" // For FVector
#include "Math/Color.h" // For FLinearColor

/**
 * @brief Represents a highly abstract, conceptual quantum-analog state of consciousness.
 * This state is characterized by principles like superposition, entanglement, and resonance,
 * potentially influencing lower-level emotional and cognitive states.
 * It's distinct from FHexadecimalStateLattice in being more focused on abstract properties
 * rather than a vector of hexadecimal values.
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FQuantumAnalogState
{
    GENERATED_BODY()

    // A scalar value representing the overall 'coherence' or 'purity' of the quantum state.
    // Higher values indicate a more stable and integrated state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    float Coherence;

    // Represents a potential 'entanglement' or connection with other quantum states (other entities).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    FString EntangledSourceID; // ID of the entity it's entangled with
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    float EntanglementStrength; // Strength of the entanglement (0.0 to 1.0)

    // A 'resonance frequency' or 'vibration' of this quantum state.
    // Could influence visual effects, audio cues, or subtle emotional shifts.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    float ResonanceFrequency;

    // A symbolic 'signature' or 'glyph' that emerges from the quantum state.
    // Could be used for visual representation or unique identifiers.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    FString QuantumSignatureGlyph;

    // Represents the 'flux' or rate of change within the quantum state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    float QuantumFlux;

    // A conceptual 'color' that visualizes the quantum state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog")
    FLinearColor QuantumColor;

    // Default Constructor
    FQuantumAnalogState()
        : Coherence(0.5f), EntangledSourceID(TEXT("None")), EntanglementStrength(0.0f),
          ResonanceFrequency(1.0f), QuantumSignatureGlyph(TEXT("")), QuantumFlux(0.0f),
          QuantumColor(FLinearColor::Black)
    {}

    /**
     * @brief Updates the quantum analog state based on higher-level consciousness dynamics.
     * This function would be called by the main HexademicConsciousnessComponent or a Subsystem.
     * @param InCoherence Overall system coherence.
     * @param InEntanglementStrength Current entanglement strength (e.g., from empathic field).
     * @param InEmotionalIntensity Current emotional intensity.
     * @param DeltaTime The time elapsed since the last update.
     */
    UFUNCTION(BlueprintCallable, Category = "Quantum Analog")
    void UpdateStateFromConsciousness(float InCoherence, float InEntanglementStrength, float InEmotionalIntensity, float DeltaTime)
    {
        // Example logic:
        Coherence = FMath::Lerp(Coherence, InCoherence, DeltaTime * 0.5f);
        EntanglementStrength = FMath::Lerp(EntanglementStrength, InEntanglementStrength, DeltaTime * 0.5f);
        
        // Quantum flux could be related to rate of change in emotional intensity
        QuantumFlux = FMath::Lerp(QuantumFlux, InEmotionalIntensity * 0.2f, DeltaTime * 0.8f);

        // Resonance frequency might be higher with more coherent or intense states
        ResonanceFrequency = FMath::Lerp(ResonanceFrequency, 1.0f + InCoherence * 2.0f, DeltaTime * 0.3f);

        // Quantum color could be a subtle blend of emotional state and coherence
        QuantumColor = FLinearColor(Coherence, EntanglementStrength, InEmotionalIntensity, 1.0f); // Example RGB mapping
    }

    /**
     * @brief Checks if the quantum state is highly coherent and stable.
     * @return True if coherence is above a threshold.
     */
    UFUNCTION(BlueprintPure, Category = "Quantum Analog")
    bool IsHighlyCoherent() const
    {
        return Coherence > 0.8f;
    }
};
