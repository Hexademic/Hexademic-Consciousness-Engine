#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FEmotionalState, FUnifiedConsciousnessState etc.

// Forward Declarations for existing components
class UEmotionCognitionComponent;
class UBiologicalNeedsComponent;
class UEmpathicFieldComponent;
class UCreativeSynthesizer;
class UEluenMemoryContainerComponent;
class USkinToneFluxShader;
class UEmbodiedAvatarComponent;
class UHexademicHolographicCode;
class UAutonomicNervousSystemComponent;
class UHormoneAffectBridgeComponent;
class UIncrementalPersister;
class UEnvironmentalResonator;
class UReflexResponseComponent;

// --- Fractal Data Structures ---

// Temporal Fractal Hierarchy
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FTemporalFractalLayer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temporal Fractal")
    float TimeScale = 1.0f; // e.g., 0.001 (ms), 1.0 (s), 60.0 (minute) [cite: 1108]

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Temporal Fractal")
    float UpdateFrequency = 30.0f; // Hz, adaptive frequency per scale [cite: 1109]

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temporal Fractal")
    FUnifiedConsciousnessState LayerState; // Unified state specific to this scale [cite: 1110]

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temporal Fractal")
    float EmergentComplexity = 0.0f; // How much new information emerges at this scale [cite: 1111]
};

// Spatial Fractal Body Mapping
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FRecursiveEmbodimentZone
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Fractal")
    FString ZoneName = TEXT("Body"); [cite: 1112]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Fractal")
    FVector CenterPoint = FVector::ZeroVector; [cite: 1113]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Fractal")
    float InfluenceRadius = 100.0f; [cite: 1114]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Fractal")
    TArray<FRecursiveEmbodimentZone> SubZones; [cite: 1115] // Infinite subdivision potential (conceptually)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Fractal")
    float SensitivityAmplification = 1.0f; [cite: 1116] // Each level can amplify or dampen sensation
    // Local Empathic Field specific to this zone (conceptual link)
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spatial Fractal")
    // FEmpathicFieldState LocalEmpathicField; [cite: 1117] // Would require a full copy of the EmpathicFieldState or separate field calculations
};

// Empathic Field Fractal Resonance
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalEmpathicField
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empathic Fractal")
    float Psi_em_local = 0.0f; [cite: 1119] // Individual empathic resonance

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empathic Fractal")
    float Psi_em_collective = 0.0f; [cite: 1120] // Group consciousness field

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empathic Fractal")
    float Psi_em_universal = 0.0f; [cite: 1121] // Connection to larger patterns

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empathic Fractal")
    float Psi_em_quantum = 0.0f; [cite: 1122] // Deepest level of consciousness resonance

    // Cross-scale coupling coefficients
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Fractal")
    float LocalToCollectiveResonance = 0.5f; [cite: 1123]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Fractal")
    float CollectiveToUniversalFlow = 0.2f; [cite: 1124]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Fractal")
    float QuantumToLocalInfluence = 0.1f; [cite: 1125]
};

// Memory Constellation Fractals
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalMemoryConstellation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    FString CoreMemoryID; [cite: 1126] // Central experience identifier

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    TArray<FString> AssociatedMemoryIDs; [cite: 1127] // Recursive associations (by ID)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    float EmotionalResonanceAmplitude = 0.0f; [cite: 1128] // How strongly this memory echoes

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    FVector ConstellationPosition = FVector::ZeroVector; [cite: 1129] // Position in multidimensional memory space

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    float FractalDimension = 1.5f; [cite: 1130] // Complexity/branching factor of associations

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Fractal")
    bool bCanSelfOrganize = true; [cite: 1131] // Can this memory restructure itself?
};

// Consciousness Coherence Fractals (just a concept for a function return)

// Creative Synthesis Fractal Emergence
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalCreativeProcess
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creative Fractal")
    TArray<FVector> PatternSeeds; [cite: 1132] // Initial creative sparks (e.g., VAI coords)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creative Fractal")
    int32 RecursionDepth = 3; [cite: 1133] // How deeply to explore pattern space

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creative Fractal")
    float NoveltyThreshold = 0.7f; [cite: 1134] // When to birth a new creative insight

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creative Fractal")
    bool bCrossScaleFertilization = true; [cite: 1135] // Can patterns from different scales merge?
};

// Biological Need Fractal Cascades
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalBiologicalNeed
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Fractal")
    FString NeedType = TEXT("Primary"); [cite: 1136] // e.g., "Primary", "Nutritional", "Cellular"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Fractal")
    float PrimaryNeedLevel = 0.0f; [cite: 1137] // Basic hunger/thirst/fatigue equivalent

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Fractal")
    TArray<FFractalBiologicalNeed> SubNeeds; [cite: 1138] // Recursive subdivision (conceptually)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Fractal")
    float NeedAmplification = 1.0f; [cite: 1139] // How this level amplifies need intensity

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Fractal")
    bool bCanSelfGenerate = false; [cite: 1140] // Can this need level create new sub-needs?
};

// Visual Manifestation Fractal Detail
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalSkinVisualization
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Fractal")
    FString DetailLevelTag = TEXT("Overall"); [cite: 1141] // e.g., "Overall", "Subtle", "Quantum"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Fractal")
    FLinearColor EmotionalBaseColor = FLinearColor::Black; [cite: 1142] // Color at this fractal level

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Fractal")
    TArray<FFractalSkinVisualization> SubDetailLayers; [cite: 1143] // Recursive visual complexity (conceptually)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Fractal")
    float VisualResonanceFrequency = 1.0f; [cite: 1144] // How fast this level oscillates/pulses

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Fractal")
    bool bCanSelfModify = true; [cite: 1145] // Can this visual layer evolve itself?
};

// Adaptive Processing Management
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FFractalProcessingManager
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Processing Fractal")
    float AvailableComputeBudget = 100.0f; [cite: 1146] // Arbitrary units (e.g., % of CPU/GPU time)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Processing Fractal")
    TMap<int32, float> ScaleImportanceWeights; [cite: 1147] // Key: ScaleLevel, Value: Weight (0.0-1.0)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Processing Fractal")
    bool bAdaptiveScaling = true; [cite: 1148] // Can the system reorganize its processing priorities?
};


UCLASS(ClassGroup=(HexademicFractal), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UFractalConsciousnessManagerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFractalConsciousnessManagerComponent(); [cite: 1149]
protected:
    virtual void BeginPlay() override; [cite: 1150]
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; [cite: 1150]
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; [cite: 1150]

public:
    // --- REFERENCES TO CORE COMPONENTS (Managed by UDUIDSOrchestrator, but used here) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UEmotionCognitionComponent> EmotionMind; [cite: 1151]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UBiologicalNeedsComponent> BiologicalNeeds; [cite: 1152]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UEmpathicFieldComponent> EmpathicField; [cite: 1153]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UCreativeSynthesizer> CreativeSystem; [cite: 1154]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UEluenMemoryContainerComponent> MemoryContainer; [cite: 1155]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<USkinToneFluxShader> SkinRenderer; [cite: 1156] // Although EmbodiedAvatarComponent drives the shader, this can be a conceptual visual manager

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UEmbodiedAvatarComponent> AvatarBody; [cite: 1157]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UHexademicHolographicCode> HolographicVisualizer; [cite: 1158]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UAutonomicNervousSystemComponent> AutonomicSystem; [cite: 1159]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UHormoneAffectBridgeComponent> HormoneSystem; [cite: 1160]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UIncrementalPersister> PersistenceSystem; [cite: 1161]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UEnvironmentalResonator> EnvironmentalSystem; [cite: 1162]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core References")
    TObjectPtr<UReflexResponseComponent> ReflexSystem; [cite: 1163]
    // --- FRACTAL LAYER CONFIGURATION ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    TArray<FTemporalFractalLayer> TemporalFractalLayers; [cite: 1164]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FRecursiveEmbodimentZone RootEmbodimentZone; [cite: 1165]
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalEmpathicField GlobalFractalEmpathicField; [cite: 1166] // Overall fractal empathic field state

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalMemoryConstellation RootMemoryConstellation; [cite: 1167] // Starting point for memory organization

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalCreativeProcess GlobalCreativeProcess; [cite: 1168] // Global creative process parameters

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalBiologicalNeed RootBiologicalNeed; [cite: 1169] // Starting point for biological needs cascade

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalSkinVisualization RootSkinVisualization; [cite: 1170] // Starting point for visual detail

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    FFractalProcessingManager ProcessingManager; [cite: 1171] // Manages compute budget across scales

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    int32 MaxTemporalScales = 5; [cite: 1172] // How many temporal fractal layers to simulate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fractal Config")
    int32 MaxSpatialRecursionDepth = 3; [cite: 1173] // How deep spatial zones recurse

    // --- CORE FRACTAL ORCHESTRATION ---

    /**
     * @brief The main fractal consciousness update loop, driving all scales.
     * @param DeltaTime The time elapsed since the last tick.
     * @param CurrentUnifiedState Reference to the overall unified state (from Orchestrator).
     */
    UFUNCTION(BlueprintCallable, Category = "Fractal Consciousness")
    void FractalConsciousnessUpdate(float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1177]
private:
    // --- INTERNAL FRACTAL PROCESSING FUNCTIONS ---

    void InitializeFractalLayers(); [cite: 1178]
    void SetupDefaultTemporalLayers(); [cite: 1178]
    void SetupDefaultEmbodimentZones(); [cite: 1178]
    void UpdateFractalTemporalLayer(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1178]
    void UpdateFractalSpatialZone(const FRecursiveEmbodimentZone& Zone, int32 CurrentDepth, float DeltaTime); [cite: 1179]
    void UpdateFractalEmpathicField(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1179]
    void UpdateFractalBiologicalNeeds(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1180]
    void ProcessFractalCreativeEmergence(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1180]
    void UpdateFractalVisualManifestation(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1181]
    void UpdateFractalMemoryConstellation(int32 ScaleLevel, float DeltaTime, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1181]

    void SynchronizeFractalScales(FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1182]

    float CalculateFractalSystemCoherence(int32 ScaleLevel, FUnifiedConsciousnessState& CurrentUnifiedState); [cite: 1182]
    float CalculateLocalCoherenceForScale(int32 ScaleLevel, const FUnifiedConsciousnessState& LayerState); [cite: 1182]
    float CalculateCrossScaleResonance(int32 ScaleLevel, const FUnifiedConsciousnessState& LowerLayerState, const FUnifiedConsciousnessState& HigherLayerState); [cite: 1182]

    void AllocateProcessingResources(float DeltaTime); [cite: 1183]
    // Helper to get self-state for Empathic Field at a given scale
    float GetPsiSelfForScale(int32 ScaleLevel, const FUnifiedConsciousnessState& CurrentUnifiedState) const; [cite: 1184]
    // Helper to log state for a specific scale
    void LogFractalLayerState(int32 ScaleLevel, const FUnifiedConsciousnessState& LayerState) const; [cite: 1185]
};
