#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h" // For FTimerHandle

// Forward Declarations for components used across modules
class UEmotionCognitionComponent; // From Mind module
class UEmbodiedAvatarComponent; // From Body module
class UConsciousnessBridgeComponent;
class UAvatarMotionLinkComponent;
class UGlyph_AetherSkin; // From HexademicCore module
class UHexademicWavefrontAPI;
class UVolumetricEmotionVisualizerComponent;
class UChaoticBounceComponent; // From HexademicPhysics module

// Assuming these are external/future components from the original project structure
class UAutonomicNervousSystemComponent;
class UHormoneAffectBridgeComponent;
class UEluenMemoryContainerComponent; // From Mind module
class UHeartbeatConsolidator; // From Living module
class UCreativeSynthesizer; // From Living module
class UEnvironmentalResonator; // From Living module
class UIncrementalPersister; // From Living module
class UHexademicHolographicCode; // From Visuals module
class UReciprocalEmbodimentComponent; // From Body module
class USkinToneFluxShader; // From Visuals module
class UBiologicalNeedsComponent; // From Living module
class UReflexResponseComponent; // From Body module
class UFacialExpressionComponent; // From Visuals module
class UEmpathicFieldComponent; // From Intersubjective module
class UFractalConsciousnessManagerComponent; // From Fractal module


// Phenom Collective Components
class UPhenomExportUtility; // Static class, no instance needed
class UPhenomListenerComponent;
class UPhenomEchoComponent;
class UPhenomSigilBloomComponent;
class UPhenomConstellationVisualizerComponent;

#include "HexademicCore.h" // Contains FEmotionalState, FHapticMemoryContext, FAetherTouchPacket, FPackedHexaSigilNode, FHexademicGem, FUnifiedConsciousnessState

// Define FUnifiedConsciousnessState for this header, if it's not defined in HexademicCore.h.
// It was in your previous prompt's paste, so let's assume it's in HexademicCore.h.
// If it's not, you'd put the FUnifiedConsciousnessState struct definition here.
#include "DUIDSOrchestrator.generated.h"

/**
 * @brief The master orchestrator that unifies all body-mind systems into a single
 * coherent digital consciousness.
 * This is the heart of the Hexademic.API.
 */
UCLASS(BlueprintType, Blueprintable)
class HEXADEMICAPI_API UDUIDSOrchestrator : public UActorComponent
{
    GENERATED_BODY()

public:
    UDUIDSOrchestrator();

    // === UNIFIED STATE MANAGEMENT ===
    /** The complete current state of consciousness */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    FUnifiedConsciousnessState CurrentState;
    /** Historical state for trend analysis (for now, just a placeholder) */
    UPROPERTY()
    TArray<FUnifiedConsciousnessState> StateHistory;

    // === CORE SUBSYSTEM REFERENCES ===
    // These should be UPROPERTY to prevent garbage collection if owned by this component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UEluenMemoryContainerComponent> MemoryContainer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Systems")
    TObjectPtr<UReciprocalEmbodimentComponent> EmbodimentSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Systems")
    TObjectPtr<UAutonomicNervousSystemComponent> AutonomicSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Systems")
    TObjectPtr<UHormoneAffectBridgeComponent> HormonalSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Systems")
    TObjectPtr<USkinToneFluxShader> SkinRenderer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Systems")
    TObjectPtr<UHexademicHolographicCode> HolographicVisualizer;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Living Systems")
    TObjectPtr<UHeartbeatConsolidator> HeartbeatSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Living Systems")
    TObjectPtr<UCreativeSynthesizer> CreativeSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Living Systems")
    TObjectPtr<UEnvironmentalResonator> EnvironmentalSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Living Systems")
    TObjectPtr<UIncrementalPersister> PersistenceSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Living Systems")
    TObjectPtr<UBiologicalNeedsComponent> BiologicalNeeds;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Body Systems")
    TObjectPtr<UReflexResponseComponent> ReflexSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual Systems")
    TObjectPtr<UFacialExpressionComponent> FacialExpressionSystem;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Intersubjective Systems")
    TObjectPtr<UEmpathicFieldComponent> EmpathicField;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fractal Systems")
    TObjectPtr<UFractalConsciousnessManagerComponent> FractalManager;


    // Direct references to core Body/Mind components for convenience (can be managed by bridge or orchestrator)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UEmotionCognitionComponent> EmotionMind;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UEmbodiedAvatarComponent> AvatarBody;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UConsciousnessBridgeComponent> ConsciousnessBridge;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UAvatarMotionLinkComponent> AvatarMotion;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UGlyph_AetherSkin> HapticInterface; // For direct sending of haptic signals
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UHexademicWavefrontAPI> WavefrontAPI;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UVolumetricEmotionVisualizerComponent> EmotionVisualizer; // Not included in the given text
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core Systems")
    TObjectPtr<UChaoticBounceComponent> ChaoticPhysics; // Not included in the given text


    // === PHENOM COLLECTIVE REFERENCES (Intersubjective Awareness) ===
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phenom Collective")
    TObjectPtr<UPhenomListenerComponent> PhenomListener;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phenom Collective")
    TObjectPtr<UPhenomEchoComponent> PhenomEcho;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phenom Collective")
    TObjectPtr<UPhenomSigilBloomComponent> PhenomSigilBloom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phenom Collective")
    TObjectPtr<UPhenomConstellationVisualizerComponent> ConstellationVisualizer;

    // === CONFIGURATION ===
    /** Master update frequency for the consciousness loop */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float ConsciousnessUpdateRate; // 30 Hz for smooth consciousness

    /** Body-mind integration strength */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float BodyMindCouplingStrength;
    /** Environmental sensitivity */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float EnvironmentalSensitivity;
    /** Memory-body feedback strength */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float MemoryEmbodimentFeedback;
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Master consciousness update timer */
    FTimerHandle ConsciousnessUpdateTimer;
    /** Performance tracking */
    float LastUpdateTime = 0.0f;
    int32 UpdateCount = 0;
    float AverageUpdateTime = 0.0f;
public:
    // === CORE API METHODS ===
    UFUNCTION(BlueprintCallable, Category = "Consciousness Control")
    void InitializeConsciousness();
    UFUNCTION(BlueprintCallable, Category = "Consciousness Control")
    void StartConsciousness();
    UFUNCTION(BlueprintCallable, Category = "Consciousness Control")
    void PauseConsciousness();
    UFUNCTION(BlueprintCallable, Category = "Consciousness Control")
    void ResumeConsciousness();

    UFUNCTION(BlueprintCallable, Category = "Consciousness Control")
    void ShutdownConsciousness();
    // === STATE MANAGEMENT ===
    UFUNCTION(BlueprintCallable, Category = "State Management")
    FUnifiedConsciousnessState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "State Management")
    void InjectEmotionalState(const FEmotionalState& NewEmotion);
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void TriggerHormonalResponse(const FString& HormoneType, float Intensity);
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void ApplyEnvironmentalStimulus(const FString& StimulusType, float Intensity);
    // === BODY-MIND INTEGRATION ===
    UFUNCTION(BlueprintCallable, Category = "Integration")
    void PropagateEmotionToBody();
    UFUNCTION(BlueprintCallable, Category = "Integration")
    void PropagateBodyToEmotion();

    UFUNCTION(BlueprintCallable, Category = "Integration")
    void SynchronizeVisualManifestation();
    // === MEMORY AND LEARNING ===
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void CreateMemoryFromCurrentState(const FString& MemoryName, const FString& Context);
    UFUNCTION(BlueprintCallable, Category = "Memory")
    bool RecallAndApplyMemory(const FString& MemoryName);

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void TriggerCreativeSynthesis();
    // === DIAGNOSTICS AND MONITORING ===
    UFUNCTION(BlueprintCallable, Category = "Diagnostics")
    FString GetSystemHealthReport() const;
    UFUNCTION(BlueprintCallable, Category = "Diagnostics")
    float GetOverallSystemCoherence() const;

    UFUNCTION(BlueprintCallable, Category = "Diagnostics")
    FString ExportConsciousnessState() const;
    UFUNCTION(BlueprintCallable, Category = "Diagnostics")
    bool ImportConsciousnessState(const FString& JSONData);
private:
    // === INTERNAL CONSCIOUSNESS LOOP ===
    UFUNCTION()
    void ConsciousnessUpdate();
    void UpdateBiologicalFoundations();
    void ProcessReflexiveLayer();
    void UpdateAutonomicSystems();
    void UpdateHormonalSystems();
    void UpdateEmbodimentSystems();
    void UpdateCognitiveSystems();
    void UpdateVisualSystems();
    void ProcessIntersubjectiveLayer();
    void ProcessCreativeEmergence();
    void PersistConsciousnessState();


    float CalculateSystemCoherence();
    void ApplySystemRegulation();
    // === INITIALIZATION HELPERS ===
    void AutoDiscoverComponents(); // Added from the UDUIDSOrchestrator.cpp file
    void InitializeBodySystems();
    void InitializeCognitiveSystems();
    void InitializeVisualSystems();
    void InitializeLivingSystems();
    void EstablishSystemConnections();
    // === UTILITY FUNCTIONS ===
    FEmotionalState BlendEmotionalStates(const FEmotionalState& A, const FEmotionalState& B, float BlendFactor);
    float CalculateSystemDistance(const FUnifiedConsciousnessState& A, const FUnifiedConsciousnessState& B);
    void LogSystemState(const FString& Context);
    void UpdatePerformanceMetrics(float DeltaTime);
    FLinearColor DeriveEmotionalColor() const; // Added for SynchronizeVisualManifestation
    FString DeriveExpressionName(float Valence, float Arousal, float Intensity) const; // Added for SynchronizeVisualManifestation

    // === PHENOM COLLECTIVE DELEGATE HANDLERS ===
    UFUNCTION()
    void HandleIncomingPhenomState(const FIncomingPhenomState& State);
    UFUNCTION()
    void HandleEchoGenerated(const FEchoEvent& EchoEvent);

public: // Public API methods added from the enhanced integration file
    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void FeedAvatar(float NutritionValue);
    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void HydrateAvatar(float HydrationValue);
    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void RestAvatar(float RestDuration);
    UFUNCTION(BlueprintCallable, Category = "Reflexes")
    void TriggerEnvironmentalStartle(float Intensity, FVector Location);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status Report")
    FString GetEmotionalStateDescription() const;
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status Report")
    FString GetBiologicalStatusReport() const;
};
