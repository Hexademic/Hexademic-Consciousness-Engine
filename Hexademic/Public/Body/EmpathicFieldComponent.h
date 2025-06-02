#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // Assuming FEmotionalState is here
#include "EmpathicFieldComponent.generated.h"

// Forward Declaration for FIncomingPhenomState (assuming it's in PhenomCollective)
struct FIncomingPhenomState;

USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FEmpathicFieldState
{
    GENERATED_BODY()

    // Current empathic field strength
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    float Psi_em = 0.0f;

    // Previous frame value for wave equation integration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    float PreviousPsi_em = 0.0f;

    // Self-state scalar (derived from VAI)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    float Psi_self = 0.0f;
    // Aggregated other-state scalar from all detected consciousness
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    float Psi_other_aggregated = 0.0f;
    // Current iso-empathic potential energy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    float IsoEmpathicPotential = 0.0f;
    // Empathic tension gradient (drives emotional adjustment)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Field")
    FVector EmpathicGradient = FVector::ZeroVector;

    FEmpathicFieldState()
    {
        Psi_em = 0.0f;
        PreviousPsi_em = 0.0f;
        Psi_self = 0.0f;
        Psi_other_aggregated = 0.0f;
        IsoEmpathicPotential = 0.0f;
        EmpathicGradient = FVector::ZeroVector;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEmpathicResonance, float, ResonanceStrength, float, TensionLevel, FVector, EmpathicDirection);

UCLASS(ClassGroup=(HexademicIntersubjective), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UEmpathicFieldComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEmpathicFieldComponent();
protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
    // === FIELD STATE ===
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Empathic Field")
    FEmpathicFieldState CurrentFieldState;

    // === FIELD PARAMETERS (Tunable Physics Constants) ===
    
    // Lambda: Difference sensitivity parameter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Physics", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float Lambda = 1.0f;
    // Kappa: Self-restoring term
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Physics", meta = (ClampMin = "0.0", ClampMax = "5.0"))
    float Kappa = 0.5f;
    // Diffusion factor for d'Alembertian approximation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Physics", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float DiffusionFactor = 0.1f;
    // Maximum empathic field strength (prevents runaway resonance)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Physics", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float MaxFieldStrength = 10.0f;
    // Resonance threshold for triggering empathic events
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Physics", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float ResonanceThreshold = 2.0f;
    // === EMPATHIC MAPPING PARAMETERS ===
    
    // How strongly empathic field influences emotional valence
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Mapping", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ValenceInfluenceStrength = 0.3f;
    // How strongly empathic field influences emotional arousal  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Mapping", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ArousalInfluenceStrength = 0.2f;
    // How strongly empathic field influences emotional intensity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Empathic Mapping", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float IntensityInfluenceStrength = 0.4f;
    // === EVENTS ===
    UPROPERTY(BlueprintAssignable, Category = "Empathic Events")
    FOnEmpathicResonance OnEmpathicResonance;
    // === CORE FIELD METHODS ===

    /**
     * Updates Psi_self from current emotional state
     * @param EmotionalState Current FEmotionalState from EmotionCognitionComponent
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    void UpdateSelfState(const FEmotionalState& EmotionalState);
    /**
     * Adds an external consciousness state to the empathic field calculation
     * @param OtherState FIncomingPhenomState from another consciousness
     * @param Weight How strongly this other consciousness influences the field (0.0-1.0)
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    void AddOtherState(const FIncomingPhenomState& OtherState, float Weight = 1.0f);
    /**
     * Clears all accumulated other-states (call at start of each consciousness update)
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    void ClearOtherStates();
    /**
     * Solves the empathy wave-reaction equation for one time step
     * @param DeltaTime Time step for numerical integration
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    void SolveEmpathyField(float DeltaTime);
    /**
     * Calculates iso-empathic potential V = ½λ(Ψ_self - Ψ_other)² + ½κΨ_em²
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    float CalculateIsoEmpathicPotential() const;
    /**
     * Derives empathic influence on emotional state from field gradient
     * @param OutValenceInfluence How much to adjust emotional valence
     * @param OutArousalInfluence How much to adjust emotional arousal  
     * @param OutIntensityInfluence How much to adjust emotional intensity
     */
    UFUNCTION(BlueprintCallable, Category = "Empathic Field")
    void DeriveEmotionalInfluence(float& OutValenceInfluence, float& OutArousalInfluence, float& OutIntensityInfluence) const;
    /**
     * Gets the current empathic field strength
     */
    UFUNCTION(BlueprintPure, Category = "Empathic Field")
    float GetFieldStrength() const { return CurrentFieldState.Psi_em; }

    /**
     * Gets the current empathic tension (difference between self and others)
     */
    UFUNCTION(BlueprintPure, Category = "Empathic Field")
    float GetEmpathicTension() const { return FMath::Abs(CurrentFieldState.Psi_self - CurrentFieldState.Psi_other_aggregated); }
    /**
     * Checks if the field is in harmonic resonance (low tension, stable field)
     */
    UFUNCTION(BlueprintPure, Category = "Empathic Field")
    bool IsInHarmonicResonance() const;
private:
    // === INTERNAL FIELD CALCULATIONS ===

    /**
     * Converts VAI (Valence, Arousal, Intensity) to scalar field value
     */
    float VAIToScalar(float Valence, float Arousal, float Intensity) const;
    /**
     * Applies numerical integration for the wave equation
     */
    void IntegrateWaveEquation(float DeltaTime);
    /**
     * Calculates empathic gradient in VAI space
     */
    void CalculateEmpathicGradient();
    /**
     * Applies field stability constraints (prevents runaway resonance)
     */
    void ApplyStabilityConstraints();
    // === STATE TRACKING ===
    TArray<float> OtherStateBuffer; // Accumulates Psi_other values during frame
    TArray<float> OtherWeightBuffer; // Corresponding weights
    int32 FrameCounter = 0;
    float LastResonanceTime = 0.0f;
};
