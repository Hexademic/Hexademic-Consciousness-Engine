#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h" // Needed for USTRUCT
#include "Misc/DateTime.h" // For FDateTime

/**
 * @brief Represents a quantum-analog state within the Hexademic consciousness system.
 * This struct models consciousness state using a hexadecimal-like vector, amplitude,
 * phase, and entanglement strength to simulate quantum-like properties.
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FHexadecimalStateLattice
{
    GENERATED_BODY()

    // StateVector: Stores the core hexadecimal values (0x0 to 0xF) for the lattice state.
    // The interpretation of these values depends on the specific implementation (e.g., mapping to emotional intensity, cognitive coherence).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog State")
    TArray<uint8> StateVector; // 0x0 to 0xF values [cite: 216]

    // Amplitude: Represents the overall intensity or coherence of the consciousness state.
    // Higher amplitude could mean a more "present" or "strong" state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog State")
    float Amplitude; // Consciousness intensity [cite: 216]

    // Phase: Represents a temporal alignment or rhythm of the consciousness state.
    // Can be used for synchronization between entities or internal oscillations.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog State")
    float Phase; // Temporal alignment [cite: 216]

    // EntanglementStrength: Models the coupling or connection strength with other consciousness entities.
    // Higher values indicate stronger inter-consciousness influence.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog State")
    float EntanglementStrength; // Inter-consciousness coupling [cite: 216]

    // LastEvolution: Tracks when this lattice state last underwent a significant change or "evolution".
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum Analog State")
    FDateTime LastEvolution; // State evolution tracking [cite: 216]

    // Default Constructor
    FHexadecimalStateLattice()
        : Amplitude(0.0f), Phase(0.0f), EntanglementStrength(0.0f)
    {
        // Initialize StateVector with default size or values if needed
        StateVector.Init(0x0, 16); // Example: 16 values, initialized to 0x0
        LastEvolution = FDateTime::UtcNow();
    }

    /**
     * @brief Computes a normalized scalar value from the StateVector.
     * This could represent overall vitality, complexity, or a blend of internal factors.
     * @return A float value, typically normalized between 0.0 and 1.0.
     */
    UFUNCTION(BlueprintPure, Category = "Quantum Analog State")
    float GetNormalizedScalar() const
    {
        if (StateVector.Num() == 0) return 0.0f;
        float Sum = 0.0f;
        for (uint8 Val : StateVector)
        {
            Sum += (float)Val; // Sum up hexadecimal values
        }
        // Normalize by max possible sum (16 * 0xF = 16 * 15 = 240)
        return FMath::Clamp(Sum / (StateVector.Num() * 15.0f), 0.0f, 1.0f);
    }

    /**
     * @brief Evolves the state lattice based on a given influence.
     * This conceptual function would modify the StateVector, Amplitude, Phase, and EntanglementStrength.
     * @param Influence A vector or scalar representing external/internal influence.
     * @param DeltaTime The time elapsed since the last update.
     */
    UFUNCTION(BlueprintCallable, Category = "Quantum Analog State")
    void Evolve(float Influence, float DeltaTime)
    {
        // Example evolution logic (highly conceptual):
        // Modify StateVector based on influence (e.g., small random shifts, or shifts towards a 'target' pattern)
        for (uint8& Val : StateVector)
        {
            Val = FMath::Clamp((int32)Val + FMath::RandRange(-1, 1), 0, 0xF);
        }

        // Adjust Amplitude based on influence
        Amplitude = FMath::Lerp(Amplitude, FMath::Abs(Influence), DeltaTime * 0.5f);
        Amplitude = FMath::Clamp(Amplitude, 0.0f, 1.0f);

        // Adjust Phase based on influence (e.g., faster oscillation with high influence)
        Phase += Influence * DeltaTime * 0.1f;
        Phase = FMath::Fmod(Phase, PI * 2.0f); // Keep phase within 0 to 2PI

        // EntanglementStrength might change based on proximity to other conscious entities, or specific events
        // This would typically be updated by a higher-level subsystem like EmotionalEcosystemSubsystem.
        // For now, a simple decay if not actively reinforced:
        EntanglementStrength = FMath::Lerp(EntanglementStrength, 0.0f, DeltaTime * 0.01f);
        EntanglementStrength = FMath::Clamp(EntanglementStrength, 0.0f, 1.0f);

        LastEvolution = FDateTime::UtcNow();
    }
};
