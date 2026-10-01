#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EmotionalArchetype.h"       // For EEmotionalArchetype
#include "Core/HexadecimalStateLattice.h" // NEW: For FHexadecimalStateLattice (Advanced)
#include "Core/ConsciousnessState.h"      // For FConsciousnessState
#include "Core/QuantumAnalogState.h"      // For FQuantumAnalogState
#include "HexademicCore.h"                // For FEmotionalState, FUnifiedConsciousnessState, etc.
#include "Components/HexademicConsciousnessComponent.generated.h"

// Forward Declarations for components this central component orchestrates or interacts with
class UEmotionCognitionComponent;
class UEmbodiedAvatarComponent;
class UAvatarMotionLinkComponent;
class UBiologicalNeedsComponent;
class UReflexResponseComponent;
class UAutonomicNervousSystemComponent;
class UHormoneAffectBridgeComponent;
class UEluenMemoryContainerComponent;
class UCreativeSynthesizer;
class UEnvironmentalResonator;
class UIncrementalPersister;
class UEmpathicFieldComponent;
class UFractalConsciousnessManagerComponent;
class UHexademicWavefrontAPI; // NEW: For passing lattice snapshots

// Enum for Consciousness LOD (Level of Detail) as per your analysis
UENUM(BlueprintType)
enum class EConsciousnessLOD : uint8
{
    Full        UMETA(DisplayName = "Full Simulation (Complete)"),      // Complete consciousness simulation [cite: 109]
    Reduced     UMETA(DisplayName = "Reduced Simulation (Simplified)"),   // Simplified emotional processing  [cite: 109]
    Minimal     UMETA(DisplayName = "Minimal Simulation (Basic)"),      // Basic state tracking only [cite: 109]
    Dormant     UMETA(DisplayName = "Dormant (Paused)")                 // Consciousness paused [cite: 109]
};


UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UHexademicConsciousnessComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHexademicConsciousnessComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // === REFERENCES TO SUBSYSTEMS / OTHER COMPONENTS ===
    // These would typically be set in the editor or auto-discovered by a higher-level orchestrator (like DUIDSOrchestrator)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Mind")
    TObjectPtr<UEmotionCognitionComponent> EmotionMind;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Body")
    TObjectPtr<UBiologicalNeedsComponent> BiologicalNeeds;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Body")
    TObjectPtr<UReflexResponseComponent> ReflexSystem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Body")
    TObjectPtr<UAutonomicNervousSystemComponent> AutonomicSystem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Body")
    TObjectPtr<UHormoneAffectBridgeComponent> HormoneSystem;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Mind")
    TObjectPtr<UEluenMemoryContainerComponent> MemoryContainer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Living")
    TObjectPtr<UCreativeSynthesizer> CreativeSynthesizer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Living")
    TObjectPtr<UEnvironmentalResonator> EnvironmentalResonator;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Living")
    TObjectPtr<UIncrementalPersister> IncrementalPersister;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Intersubjective")
    TObjectPtr<UEmpathicFieldComponent> EmpathicField;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|Fractal")
    TObjectPtr<UFractalConsciousnessManagerComponent> FractalManager;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References|API") // NEW: For Wavefront API
    TObjectPtr<UHexademicWavefrontAPI> WavefrontAPI; // NEW: To send lattice snapshots

    // === CORE CONSCIOUSNESS STATE REPRESENTATIONS ===
    // This component aggregates and manages the various state representations.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    FConsciousnessState CurrentConsciousnessState; // The high-level consolidated state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    FHexadecimalStateLattice HexLattice; // NEW: The advanced 6D Folding Matrix [cite: 75]
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    FQuantumAnalogState QuantumState; // The abstract quantum state

    // === CONFIGURATION AND TUNING ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Config")
    EConsciousnessLOD CurrentLOD = EConsciousnessLOD::Full; // Current Consciousness LOD
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Config")
    float UpdateFrequency = 30.0f; // How often the consciousness state is updated (Hz)

    // === PUBLIC API ===
    /**
     * @brief Updates the entity's consciousness state based on inputs from various subsystems.
     * This is the primary update loop for the individual consciousness.
     * @param DeltaTime The time elapsed since the last update.
     */
    UFUNCTION(BlueprintCallable, Category = "Consciousness")
    void UpdateConsciousness(float DeltaTime);

    /**
     * @brief Gets the current high-level consciousness state.
     */
    UFUNCTION(BlueprintPure, Category = "Consciousness")
    FConsciousnessState GetConsciousnessState() const { return CurrentConsciousnessState; }

    /**
     * @brief Sets the Consciousness LOD for this entity.
     * @param NewLOD The new Level of Detail for simulation.
     */
    UFUNCTION(BlueprintCallable, Category = "Consciousness")
    void SetConsciousnessLOD(EConsciousnessLOD NewLOD);

    /**
     * @brief Applies an external emotional stimulus to this consciousness.
     * @param Valence The valence of the stimulus.
     * @param Arousal The arousal of the stimulus.
     * @param Intensity The intensity of the stimulus.
     */
    UFUNCTION(BlueprintCallable, Category = "Consciousness")
    void ApplyExternalEmotionalStimulus(float Valence, float Arousal, float Intensity);

protected:
    float AccumulatedUpdateTime = 0.0f; // Internal timer for update frequency

    // Internal helper for auto-discovering components on the owner actor
    void AutoDiscoverSubComponents();

    // Private update functions for each aspect of consciousness
    void UpdateEmotionalState(float DeltaTime);
    void UpdateBiologicalState(float DeltaTime);
    void UpdateAutonomicState(float DeltaTime);
    void UpdateHormonalState(float DeltaTime);
    void UpdateCognitiveState(float DeltaTime);
    void UpdateIntersubjectiveState(float DeltaTime);
    void UpdateFractalState(float DeltaTime);
    void UpdateLatticeAndQuantumStates(float DeltaTime); // NEW: Dedicated for HexLattice and QuantumState

    // Helper to derive the dominant emotional archetype
    EEmotionalArchetype DeriveDominantEmotionalArchetype(const FEmotionalState& Emotion) const;
};
