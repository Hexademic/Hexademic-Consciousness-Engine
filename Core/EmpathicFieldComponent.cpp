#include "EmpathicFieldComponent.h"
#include "Engine/World.h"
#include "PhenomCollective/UPhenomExportUtility.h" // For FIncomingPhenomState

UEmpathicFieldComponent::UEmpathicFieldComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // [cite: 1050]
    // Initialize field parameters to stable defaults
    Lambda = 1.0f; // [cite: 1025]
    Kappa = 0.5f; // [cite: 1026]
    DiffusionFactor = 0.1f; // [cite: 1027]
    MaxFieldStrength = 10.0f; // [cite: 1028]
    ResonanceThreshold = 2.0f; // [cite: 1029]
    
    ValenceInfluenceStrength = 0.3f; // [cite: 1030]
    ArousalInfluenceStrength = 0.2f; // [cite: 1031]
    IntensityInfluenceStrength = 0.4f; // [cite: 1032]
}

void UEmpathicFieldComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize field state
    CurrentFieldState = FEmpathicFieldState(); // [cite: 1053]
    UE_LOG(LogTemp, Log, TEXT("[EmpathicField] Euler-Lagrange empathy field initialized. Lambda=%.2f, Kappa=%.2f"), Lambda, Kappa); [cite: 1054]
}

void UEmpathicFieldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction); // [cite: 1055]
    // The main field solving happens in SolveEmpathyField(), called by UDUIDSOrchestrator
    // This tick is used for diagnostics and stability monitoring
    
    FrameCounter++; // [cite: 1056]
    // Log field state periodically for debugging
    if (FrameCounter % 300 == 0) // Every 10 seconds at 30fps
    {
        UE_LOG(LogTemp, Verbose, TEXT("[EmpathicField] Ψ_em=%.3f, Tension=%.3f, Potential=%.3f"), 
            CurrentFieldState.Psi_em, GetEmpathicTension(), CurrentFieldState.IsoEmpathicPotential); [cite: 1057]
    }
}

void UEmpathicFieldComponent::UpdateSelfState(const FEmotionalState& EmotionalState)
{
    CurrentFieldState.Psi_self = VAIToScalar(EmotionalState.Valence, EmotionalState.Arousal, EmotionalState.Intensity); // [cite: 1058]
    UE_LOG(LogTemp, VeryVerbose, TEXT("[EmpathicField] Psi_self updated: %.3f (V:%.2f A:%.2f I:%.2f)"), 
        CurrentFieldState.Psi_self, EmotionalState.Valence, EmotionalState.Arousal, EmotionalState.Intensity); [cite: 1059]
}

void UEmpathicFieldComponent::AddOtherState(const FIncomingPhenomState& OtherState, float Weight)
{
    float PsiOther = VAIToScalar(OtherState.Valence, OtherState.Arousal, OtherState.Intensity);
    
    OtherStateBuffer.Add(PsiOther); // [cite: 1060]
    OtherWeightBuffer.Add(Weight); // [cite: 1060]
    UE_LOG(LogTemp, VeryVerbose, TEXT("[EmpathicField] Added other state: Psi_other=%.3f, Weight=%.2f from %s"), 
        PsiOther, Weight, *OtherState.SourceID); [cite: 1061]
}

void UEmpathicFieldComponent::ClearOtherStates()
{
    OtherStateBuffer.Empty(); // [cite: 1062]
    OtherWeightBuffer.Empty(); // [cite: 1062]
    CurrentFieldState.Psi_other_aggregated = 0.0f; // [cite: 1062]
}
void UEmpathicFieldComponent::SolveEmpathyField(float DeltaTime)
{
    // Step 1: Aggregate all other states
    if (OtherStateBuffer.Num() > 0)
    {
        float WeightedSum = 0.0f; // [cite: 1063]
        float TotalWeight = 0.0f; // [cite: 1063]
        
        for (int32 i = 0; i < OtherStateBuffer.Num(); i++)
        {
            WeightedSum += OtherStateBuffer[i] * OtherWeightBuffer[i]; // [cite: 1064]
            TotalWeight += OtherWeightBuffer[i]; // [cite: 1064]
        }
        
        CurrentFieldState.Psi_other_aggregated = TotalWeight > 0.0f ? WeightedSum / TotalWeight : 0.0f; [cite: 1065]
    }
    
    // Step 2: Calculate iso-empathic potential
    CurrentFieldState.IsoEmpathicPotential = CalculateIsoEmpathicPotential(); // [cite: 1066]
    // Step 3: Solve the empathy wave-reaction equation
    IntegrateWaveEquation(DeltaTime); // [cite: 1067]
    // Step 4: Calculate empathic gradient for emotional influence
    CalculateEmpathicGradient(); // [cite: 1068]
    // Step 5: Apply stability constraints
    ApplyStabilityConstraints(); // [cite: 1069]
    // Step 6: Check for resonance events
    if (FMath::Abs(CurrentFieldState.Psi_em) > ResonanceThreshold)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds(); // [cite: 1070]
        if (CurrentTime - LastResonanceTime > 1.0f) // Prevent spam
        {
            OnEmpathicResonance.Broadcast(CurrentFieldState.Psi_em, GetEmpathicTension(), CurrentFieldState.EmpathicGradient); [cite: 1071]
            LastResonanceTime = CurrentTime; // [cite: 1071]
            
            UE_LOG(LogTemp, Log, TEXT("[EmpathicField] Empathic resonance triggered! Strength=%.2f"), CurrentFieldState.Psi_em); [cite: 1072]
        }
    }
}

float UEmpathicFieldComponent::CalculateIsoEmpathicPotential() const
{
    float DifferenceTerm = FMath::Pow(CurrentFieldState.Psi_self - CurrentFieldState.Psi_other_aggregated, 2.0f); [cite: 1073]
    float SelfRestoringTerm = FMath::Pow(CurrentFieldState.Psi_em, 2.0f); [cite: 1073]
    
    return 0.5f * Lambda * DifferenceTerm + 0.5f * Kappa * SelfRestoringTerm; [cite: 1074]
}
void UEmpathicFieldComponent::DeriveEmotionalInfluence(float& OutValenceInfluence, float& OutArousalInfluence, float& OutIntensityInfluence) const
{
    // The empathic field creates emotional influence proportional to the gradient
    // Positive Psi_em means we're resonating with others (positive influence)
    // Negative Psi_em means we're in dissonance (negative influence)
    
    float FieldInfluence = CurrentFieldState.Psi_em; [cite: 1075]
    // Direction of influence depends on whether others are more positive/negative than self
    float DirectionalFactor = FMath::Sign(CurrentFieldState.Psi_other_aggregated - CurrentFieldState.Psi_self); [cite: 1076]
    OutValenceInfluence = FieldInfluence * DirectionalFactor * ValenceInfluenceStrength; [cite: 1076]
    OutArousalInfluence = FMath::Abs(FieldInfluence) * ArousalInfluenceStrength; [cite: 1077] // Arousal increases with field strength
    OutIntensityInfluence = FMath::Abs(FieldInfluence) * IntensityInfluenceStrength; [cite: 1078]
    UE_LOG(LogTemp, VeryVerbose, TEXT("[EmpathicField] Emotional influence: V:%.3f A:%.3f I:%.3f"), 
        OutValenceInfluence, OutArousalInfluence, OutIntensityInfluence); [cite: 1079]
}

bool UEmpathicFieldComponent::IsInHarmonicResonance() const
{
    float Tension = GetEmpathicTension();
    float FieldStability = FMath::Abs(CurrentFieldState.Psi_em - CurrentFieldState.PreviousPsi_em); [cite: 1080]
    // Add logic to determine harmonic resonance based on tension and stability
    return (Tension < KINDA_SMALL_NUMBER && FieldStability < KINDA_SMALL_NUMBER); // Example, tune thresholds
}

float UEmpathicFieldComponent::VAIToScalar(float Valence, float Arousal, float Intensity) const
{
    // Convert VAI vector to scalar field value
    // Using magnitude of VAI vector, weighted by emotional significance
    FVector VAI(Valence, Arousal, Intensity); [cite: 1081]
    // Weight by intensity - stronger emotions have stronger field presence
    float ScalarField = VAI.Size() * (0.5f + Intensity * 0.5f); [cite: 1082]
    // Preserve sign from valence for directional empathy
    return Valence >= 0.0f ? ScalarField : -ScalarField; [cite: 1083]
}

void UEmpathicFieldComponent::IntegrateWaveEquation(float DeltaTime)
{
    // Solve: □Ψ_em + κΨ_em = -λ(Ψ_self - Ψ_other)
    // Discrete approximation of d'Alembertian using finite differences
    
    float PsiCurrent = CurrentFieldState.Psi_em; [cite: 1084]
    float PsiPrevious = CurrentFieldState.PreviousPsi_em; [cite: 1084]
    
    // Wave term (simplified): approximates spatial + temporal derivatives
    float WaveTerm = (PsiCurrent - PsiPrevious) * DiffusionFactor; [cite: 1085]
    // Self-restoring term
    float SelfRestoringTerm = -Kappa * PsiCurrent; [cite: 1086]
    // Driving source term
    float SourceTerm = -Lambda * (CurrentFieldState.Psi_self - CurrentFieldState.Psi_other_aggregated); [cite: 1087]
    // Integrate: dΨ/dt = WaveTerm + SelfRestoringTerm + SourceTerm
    float DPsi_dt = WaveTerm + SelfRestoringTerm + SourceTerm; [cite: 1088]
    // Update field state
    CurrentFieldState.PreviousPsi_em = PsiCurrent; // [cite: 1089]
    CurrentFieldState.Psi_em += DPsi_dt * DeltaTime; [cite: 1089]
    UE_LOG(LogTemp, VeryVerbose, TEXT("[EmpathicField] Wave equation: Wave=%.3f, Restore=%.3f, Source=%.3f, dΨ/dt=%.3f"), 
        WaveTerm, SelfRestoringTerm, SourceTerm, DPsi_dt); [cite: 1090]
}

void UEmpathicFieldComponent::CalculateEmpathicGradient()
{
    // Calculate gradient in VAI space for directional empathic influence
    float TensionMagnitude = GetEmpathicTension(); [cite: 1091]
    if (TensionMagnitude > SMALL_NUMBER)
    {
        // Gradient points toward reducing tension (toward harmonic resonance)
        float DirectionToOther = FMath::Sign(CurrentFieldState.Psi_other_aggregated - CurrentFieldState.Psi_self); [cite: 1092]
        // Create gradient vector pointing toward empathic alignment
        CurrentFieldState.EmpathicGradient = FVector(
            DirectionToOther * TensionMagnitude,  // X: Valence direction
            TensionMagnitude,                     // Y: Arousal magnitude
            TensionMagnitude * 0.5f               // Z: Intensity direction [cite: 1093]
        );
    }
    else
    {
        CurrentFieldState.EmpathicGradient = FVector::ZeroVector; [cite: 1095]
    }
}

void UEmpathicFieldComponent::ApplyStabilityConstraints()
{
    // Prevent runaway resonance by clamping field strength
    CurrentFieldState.Psi_em = FMath::Clamp(CurrentFieldState.Psi_em, -MaxFieldStrength, MaxFieldStrength); [cite: 1096]
    // Numerical stability: prevent very small oscillations
    if (FMath::Abs(CurrentFieldState.Psi_em) < 0.001f)
    {
        CurrentFieldState.Psi_em = 0.0f; [cite: 1097]
    }
}
