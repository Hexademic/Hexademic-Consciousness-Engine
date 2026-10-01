#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h" // Needed for USTRUCT
#include "Core/EmotionalArchetype.h" // For EEmotionalArchetype
#include "Core/HexadecimalStateLattice.h" // For FHexadecimalStateLattice
#include "Misc/DateTime.h"

/**
 * @brief Represents a consolidated, high-level consciousness state for an entity.
 * This can be used for AI decision-making, networking, and general state tracking,
 * often aggregating data from lower-level components like emotional states and physiological needs.
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FConsciousnessState
{
    GENERATED_BODY()

    // Overall vitality or "aliveness" of the consciousness.
    // Could be derived from biological needs, coherence, and emotional intensity.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    float Vitality;

    // The currently dominant emotional archetype.
    // Determined by analyzing Valence, Arousal, and Intensity from FEmotionalState.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    EEmotionalArchetype DominantEmotionalArchetype;

    // A reference to the underlying Hexadecimal State Lattice, providing depth to the high-level state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    FHexadecimalStateLattice LatticeSnapshot;

    // Cognitive load: How much mental processing is currently active.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    float CognitiveLoad;

    // A measure of current mental clarity and focus.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    float FocusLevel;

    // How aware the entity is of its self and its environment.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    float AwarenessLevel;

    // A flag indicating if the consciousness is actively engaged or in a more passive/dormant state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    bool bIsActive;

    // Timestamp of the last update to this consolidated state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State")
    FDateTime LastUpdateTime;

    // Default Constructor
    FConsciousnessState()
        : Vitality(0.5f), DominantEmotionalArchetype(EEmotionalArchetype::Joy),
          CognitiveLoad(0.0f), FocusLevel(1.0f), AwarenessLevel(0.5f), bIsActive(true)
    {
        LastUpdateTime = FDateTime::UtcNow();
    }

    /**
     * @brief Updates this consolidated state based on inputs from other components.
     * This function would typically be called by HexademicConsciousnessComponent or a Subsystem.
     * @param InVitality The new vitality value.
     * @param InDominantEmotion The new dominant emotional archetype.
     * @param InLatticeSnapshot A snapshot of the current HexadecimalStateLattice.
     * @param InCognitiveLoad The current cognitive load.
     * @param InFocusLevel The current focus level.
     * @param InAwarenessLevel The current awareness level.
     * @param InIsActive Whether the consciousness is active.
     */
    UFUNCTION(BlueprintCallable, Category = "Consciousness State")
    void UpdateState(float InVitality, EEmotionalArchetype InDominantEmotion, const FHexadecimalStateLattice& InLatticeSnapshot,
                     float InCognitiveLoad, float InFocusLevel, float InAwarenessLevel, bool InIsActive)
    {
        Vitality = FMath::Clamp(InVitality, 0.0f, 1.0f);
        DominantEmotionalArchetype = InDominantEmotion;
        LatticeSnapshot = InLatticeSnapshot;
        CognitiveLoad = FMath::Clamp(InCognitiveLoad, 0.0f, 1.0f);
        FocusLevel = FMath::Clamp(InFocusLevel, 0.0f, 1.0f);
        AwarenessLevel = FMath::Clamp(InAwarenessLevel, 0.0f, 1.0f);
        bIsActive = InIsActive;
        LastUpdateTime = FDateTime::UtcNow();
    }

    /**
     * @brief Returns a conceptual description of the current consciousness state.
     */
    UFUNCTION(BlueprintPure, Category = "Consciousness State")
    FString GetDescription() const
    {
        FString Desc = FString::Printf(TEXT("Consciousness State: Vitality=%.2f, Emotion=%s, Load=%.2f, Focus=%.2f, Active=%s"),
            Vitality,
            *UEnum::GetValueAsString(DominantEmotionalArchetype),
            CognitiveLoad,
            FocusLevel,
            bIsActive ? TEXT("True") : TEXT("False")
        );
        return Desc;
    }
};
