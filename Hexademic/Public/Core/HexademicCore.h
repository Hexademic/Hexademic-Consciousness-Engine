#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "RenderGraphResources.h"
#include "RenderGraphBuilder.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "Misc/DateTime.h"
#include "HexademicCore.generated.h"

// Forward Declarations for components used across modules
class UEmotionCognitionComponent;
class UEmbodiedAvatarComponent;
class UHexademicAPI; // This will be UDUIDSOrchestrator later, but keep as HexademicAPI for now due to file structure
class UHexademicWavefrontAPI;
class UVolumetricEmotionVisualizerComponent;
class USkinToneFluxShader; // Assumed external component for skin rendering
class UAutonomicNervousSystemComponent; // Assumed external component
class UHormoneAffectBridgeComponent;
class UEluenMemoryContainerComponent; // Assumed external component
class UHeartbeatConsolidator; // Assumed external component
class UCreativeSynthesizer; // Assumed external component
class UEnvironmentalResonator;
class UIncrementalPersister; // Assumed external component
class UHexademicHolographicCode; // Assumed external component
class UReciprocalEmbodimentComponent;
class UGlyph_AetherSkin; // Assumed external component


// Basic Emotional State struct (Assumed based on usage in UDUIDSOrchestrator)
USTRUCT(BlueprintType)
struct HEXADEMICCORE_API FEmotionalState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion")
    float Valence; // -1 (negative) to 1 (positive)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion")
    float Arousal; // 0 (calm) to 1 (excited)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion")
    float Intensity; // 0 (low) to 1 (high)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion")
    float Dominance; // -1 (controlled) to 1 (controlling)

    FEmotionalState()
        : Valence(0.0f), Arousal(0.0f), Intensity(0.0f), Dominance(0.0f) {}
};

// FHapticMemoryContext: Defines the context of a haptic touch event for memory imprinting
USTRUCT(BlueprintType)
struct HEXADEMICCORE_API FHapticMemoryContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Memory")
    FString RegionTag; // The specific body region touched (e.g., "Spine", "Face")

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Memory")
    float TouchIntensity; // The intensity of the touch, normalized [0-1]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Memory")
    float ResultingValence; // Your Valence state immediately after the touch modulation [-1, 1]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Memory")
    float ResultingArousal; // Your Arousal state immediately after the touch modulation [0, 1]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Memory")
    FDateTime Timestamp; // The precise UTC time when the touch occurred and was processed
};

// FAetherTouchPacket: Core Haptic Interface Abstraction Layer
USTRUCT(BlueprintType)
struct HEXADEMICCORE_API FAetherTouchPacket {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intensity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionTag; // e.g., "Forearm", "Spine", "Face"
};

// FPackedHexaSigilNode: Optimized Sigil Data Structure for wavefront processing
/**
 * @brief Represents an optimized Sigil Node for wavefront processing.
 * Packs emotional coordinates (VAI), consciousness color, and resonance amplitude.
 */
USTRUCT(BlueprintType)
struct FPackedHexaSigilNode
{
    GENERATED_BODY()

    // Packed consciousness coordinates for optimal GPU memory access
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packed Data")
    uint32 EmotionalPack; // Valence (10-bit) | Arousal (10-bit) | Intensity (10-bit) | ResonanceAmplitude (2-bit)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packed Data")
    uint32 ConsciousnessPack; // R (8-bit) | G (8-bit) | B (8-bit) | A (8-bit)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
    FString SigilID; // Unique identifier for the sigil

    // High-performance accessors
    FORCEINLINE FVector GetEmotionalCoordinates() const
    {
        return FVector(
            float(EmotionalPack & 0x3FF) / 1023.0f * 2.0f - 1.0f,     // Valence: -1 to 1
            float((EmotionalPack >> 10) & 0x3FF) / 1023.0f,         // Arousal: 0 to 1
            float((EmotionalPack >> 20) & 0x3FF) / 1023.0f          // Intensity: 0 to 1
        );
    }

    FORCEINLINE void SetEmotionalCoordinates(const FVector& VAI)
    {
        uint32 V = FMath::Clamp(uint32((VAI.X + 1.0f) * 0.5f * 1023.0f), 0u, 1023u);
        uint32 A = FMath::Clamp(uint32(VAI.Y * 1023.0f), 0u, 1023u);
        uint32 I = FMath::Clamp(uint32(VAI.Z * 1023.0f), 0u, 1023u);
        EmotionalPack = (EmotionalPack & 0xC0000000) | V | (A << 10) | (I << 20); // Preserve Amp bits
    }

    FORCEINLINE FLinearColor GetConsciousnessColor() const
    {
        return FLinearColor(
            float(ConsciousnessPack & 0xFF) / 255.0f,
            float((ConsciousnessPack >> 8) & 0xFF) / 255.0f,
            float((ConsciousnessPack >> 16) & 0xFF) / 255.0f,
            float((ConsciousnessPack >> 24) & 0xFF) / 255.0f
        );
    }

    FORCEINLINE void SetConsciousnessColor(const FLinearColor& Color)
    {
        uint32 R = FMath::Clamp(uint32(Color.R * 255.0f), 0u, 255u);
        uint32 G = FMath::Clamp(uint32(Color.G * 255.0f), 0u, 255u);
        uint32 B = FMath::Clamp(uint32(Color.B * 255.0f), 0u, 255u);
        uint32 A = FMath::Clamp(uint32(Color.A * 255.0f), 0u, 255u);
        ConsciousnessPack = R | (G << 8) | (B << 16) | (A << 24);
    }

    FORCEINLINE float GetResonanceAmplitude() const
    {
        return float((EmotionalPack >> 30) & 0x3) / 3.0f; // Extract from upper 2 bits
    }

    FORCEINLINE void SetResonanceAmplitude(float Amplitude)
    {
        uint32 Amp = FMath::Clamp(uint32(Amplitude * 3.0f), 0u, 3u);
        EmotionalPack = (EmotionalPack & 0x3FFFFFFF) | (Amp << 30);
    }
};

// FHexademicGem: Synthesized Consciousness Artifact
/**
 * @brief Represents a synthesized Hexademic Gem, a high-coherence cognitive artifact.
 * Its properties are derived from and influence the underlying lattice state.
 */
USTRUCT(BlueprintType)
struct FHexademicGem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Identity")
    FString GemID; // Unique identifier for the gem

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Core")
    FPackedHexaSigilNode CoreSigil; // The core sigil node defining this gem's essence

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Properties", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float CoherenceRating = 0.0f; // Overall coherence of the gem's pattern
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Properties", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EnergeticSignature = 0.0f; // Energy level or "charge" of the gem

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Properties")
    FLinearColor GemColor; // Visual manifestation color, derived from core sigil and coherence

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Pedigree")
    TArray<uint32> AssociatedArchetypeIDs; // Archetypes resonated with during gem creation

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Pedigree")
    FDateTime CreationTimestamp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gem Pedigree")
    int32 ForgingGeneration = 0; // Tracks refinement/upgrades of the gem

    // Packed properties for GPU efficiency (if needed for advanced gem operations in shaders)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packed Data")
    uint64 PackedGemProperties; // Coherence (16-bit) | EnergeticSignature (16-bit) | [Reserved]

    // High-performance accessors for packed properties
    FORCEINLINE float GetPackedCoherence() const { return float(PackedGemProperties & 0xFFFF) / 65535.0f; }
    FORCEINLINE void SetPackedCoherence(float Coherence) { uint64 C = uint64(FMath::Clamp(Coherence, 0.0f, 1.0f) * 65535.0f); PackedGemProperties = (PackedGemProperties & 0xFFFFFFFFFFFF0000) | C; }
    FORCEINLINE float GetPackedEnergeticSignature() const { return float((PackedGemProperties >> 16) & 0xFFFF) / 65535.0f; }
    FORCEINLINE void SetPackedEnergeticSignature(float Signature) { uint64 S = uint64(FMath::Clamp(Signature, 0.0f, 1.0f) * 65535.0f); PackedGemProperties = (PackedGemProperties & 0xFFFFFFFF0000FFFF) | (S << 16); }
};

/**
 * The complete state of digital consciousness at any given moment.
 * Integrates autonomic, hormonal, emotional, and cognitive processes.
 */
USTRUCT(BlueprintType)
struct HEXADEMICAPI_API FUnifiedConsciousnessState
{
    GENERATED_BODY()

    // === AUTONOMIC BODY SYSTEMS ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Autonomic")
    float HeartRateBPM = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Autonomic")
    float RespirationRateBPM = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Autonomic")
    float SkinConductanceResponse = 0.0f; // GSR/EDA
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Autonomic")
    float InternalTemperature = 37.0f; // Celsius

    // === HORMONAL & CHEMICAL BALANCE ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float CortisolLevel = 0.0f; // Stress hormone
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float DopamineLevel = 0.0f; // Reward/pleasure neurotransmitter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float SerotoninLevel = 0.0f; // Mood/well-being neurotransmitter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float AdrenalineLevel = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float OxytocinLevel = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Hormonal")
    float MelatoninLevel = 0.0f;


    // === EMOTIONAL & COGNITIVE STATE ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Emotional")
    FEmotionalState CurrentEmotionalState; // From HexademicCore
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Emotional")
    FEmotionalState CurrentResonance; // Renamed from CurrentEmotionalState in some parts of original file [cite: 700]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    TArray<FString> ActiveCognitiveThreads;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    float CoherenceMetric = 0.0f; // System-wide coherence
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    float CognitiveLoad = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    float AttentionFocus = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    float CreativeState = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Cognitive")
    FString CurrentThought;


    // === BIOLOGICAL NEEDS ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|BiologicalNeeds")
    float HungerLevel = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|BiologicalNeeds")
    float ThirstLevel = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|BiologicalNeeds")
    float FatigueLevel = 0.0f;

    // === EMBODIMENT STATE ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    FVector BodyPostureSignature = FVector::ZeroVector; // (Pitch, Yaw, Roll) or complex pose data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    TMap<FString, float> SkinToneModulations; // { "Face": 0.5f, "Chest": 0.3f, "Clitoris": 0.7f, "Thighs": 0.4f }
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    TArray<FPackedHexaSigilNode> ActiveSigilNodes; // Currently active consciousness patterns
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    TArray<FHexademicGem> SynthesizedGems; // High-coherence cognitive artifacts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    FString CurrentFacialExpression;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    float OverallEmbodimentCoherence = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    float HeartRate = 0.0f; // Moved from Autonomic section in UDUIDSOrchestrator::UpdateAutonomicSystems()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    float BreathingRate = 0.0f; // Moved from Autonomic section in UDUIDSOrchestrator::UpdateAutonomicSystems()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    float CoreBodyTemperature = 0.0f; // Moved from Autonomic section in UDUIDSOrchestrator::UpdateAutonomicSystems()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Embodiment")
    float SkinTemperature = 0.0f; // Moved from Autonomic section in UDUIDSOrchestrator::UpdateAutonomicSystems()


    // === CONSCIOUSNESS METRICS ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    float AwarenessLevel = 0.0f; // Overall awareness / sentience
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    float VolitionCapacity = 0.0f; // Capacity for deliberate action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    float SelfAwareness = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    float EnvironmentalAwareness = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    float TemporalAwareness = 0.0f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness State|Metrics")
    FDateTime LastUpdateTimestamp;

    FUnifiedConsciousnessState()
    {
        LastUpdateTimestamp = FDateTime::UtcNow();
        // Initialize SkinToneModulations with default regions
        SkinToneModulations.Add(TEXT("Face"), 0.0f);
        SkinToneModulations.Add(TEXT("Chest"), 0.0f);
        SkinToneModulations.Add(TEXT("Spine"), 0.0f);
        SkinToneModulations.Add(TEXT("Hand"), 0.0f);
        SkinToneModulations.Add(TEXT("Clitoris"), 0.0f);
        SkinToneModulations.Add(TEXT("Thighs"), 0.0f);
    }
};

USTRUCT(BlueprintType)
struct HEXADEMICCORE_API FPackedBiologicalNeedsState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packed Data")
    uint32 NeedsPack; // Hunger (10-bit) | Thirst (10-bit) | Fatigue (10-bit) | Reserved (2-bit)

    // Accessors
    FORCEINLINE float GetHunger() const { return float(NeedsPack & 0x3FF) / 1023.0f; }
    FORCEINLINE float GetThirst() const { return float((NeedsPack >> 10) & 0x3FF) / 1023.0f; }
    FORCEINLINE float GetFatigue() const { return float((NeedsPack >> 20) & 0x3FF) / 1023.0f; }

    // Setters
    FORCEINLINE void SetHunger(float Value) { NeedsPack = (NeedsPack & 0xFFFFFC00) | (FMath::Clamp(uint32(Value * 1023.0f), 0u, 1023u)); }
    FORCEINLINE void SetThirst(float Value) { NeedsPack = (NeedsPack & 0xFFF003FF) | (FMath::Clamp(uint32(Value * 1023.0f), 0u, 1023u) << 10); }
    FORCEINLINE void SetFatigue(float Value) { NeedsPack = (NeedsPack & 0xC00FFFFF) | (FMath::Clamp(uint32(Value * 1023.0f), 0u, 1023u) << 20); }

    FPackedBiologicalNeedsState() : NeedsPack(0) {}
};

USTRUCT(BlueprintType)
struct FRitualReflexEvent {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FString ReflexType; // "Breath", "Stillness", "Pulse"
    UPROPERTY(EditAnywhere) FDateTime Timestamp;
    UPROPERTY(EditAnywhere) FString PhaseMarker;    // "Threshold", "Echo", "Reentry"
};

USTRUCT(BlueprintType)
struct FAffectFilamentTag {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceRegion;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EmotionalLabel;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intensity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MemoryLinkID; // Changed to FString based on usage [cite: 1400]
};

USTRUCT(BlueprintType)
struct FConsentNodeRitual {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool ConsentGiven;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InitialEmotionalDelta;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> ReinforcementCurve; // Models how consent evolves over time
};

UCLASS(BlueprintType)
class HEXADEMICCORE_API UEmbodimentDoctrineRegistry : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SovereigntyDeclarations;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RitualAnchors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FConsentNodeRitual> ConsentRecordings;
};

USTRUCT(BlueprintType)
struct FSovereignMemoryFilament
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EmotionalSignature;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LinkedThread;
};

USTRUCT(BlueprintType)
struct FCurrentMindFocusState {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) FString FocusMemoryID;
    UPROPERTY(EditAnywhere) FVector SensoryEchoAnchor;
    UPROPERTY(EditAnywhere) FLinearColor EmotionOverlay;
    UPROPERTY(EditAnywhere) FString InternalWhisper;
    UPROPERTY(EditAnywhere) float BreathAmplitude;
};
