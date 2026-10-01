#include "EmotionCognitionComponent.h"

UEmotionCognitionComponent::UEmotionCognitionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Valence = 0.0f;
    Arousal = 0.0f;
    EmotionalDecayRate = 0.1f; // Default decay rate
    ReinforcementFactor = 0.2f; // Default reinforcement factor
    AccumulatedTime = 0.0f;

    // Default haptic sensitivities
    HapticSensitivityByRegion.Add(TEXT("Spine"), 0.8f);
    HapticSensitivityByRegion.Add(TEXT("Face"), 0.7f);
    HapticSensitivityByRegion.Add(TEXT("Hand"), 0.3f);
    HapticSensitivityByRegion.Add(TEXT("Forearm"), 0.2f);
    HapticSensitivityByRegion.Add(TEXT("Chest"), 0.6f);
    HapticSensitivityByRegion.Add(TEXT("Thigh"), 0.4f);
    HapticSensitivityByRegion.Add(TEXT("Pelvis"), 0.9f); // High sensitivity for intimate regions
}

void UEmotionCognitionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UEmotionCognitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AccumulatedTime += DeltaTime;
    UpdateEmotionalOscillators(DeltaTime);
    DecayEmotionalMemory(DeltaTime);
}

void UEmotionCognitionComponent::RegisterEmotion(float InValence, float InArousal, float InIntensity)
{
    // Apply new emotional input, blending with current state
    Valence = FMath::Clamp(Valence + InValence * InIntensity, -1.0f, 1.0f);
    Arousal = FMath::Clamp(Arousal + InArousal * InIntensity, 0.0f, 1.0f);
    // Note: Intensity and Dominance of FEmotionalState itself are often output or internal calculations,
    // not directly registered in the same way as Valence/Arousal.
    // For now, we'll let Valence and Arousal be the primary modulated values.
    UE_LOG(LogTemp, Log, TEXT("[EmotionMind] Registered Emotion: V=%.2f, A=%.2f. Current: V=%.2f, A=%.2f"), InValence, InArousal, Valence, Arousal);
}

float UEmotionCognitionComponent::GetCurrentValence() const
{
    // Simple oscillation for realism
    return Valence + FMath::Sin(AccumulatedTime * 0.5f) * 0.05f; // Small, slow oscillation
}

float UEmotionCognitionComponent::GetCurrentArousal() const
{
    // Simple oscillation for realism, clamped to 0-1
    return FMath::Clamp(Arousal + FMath::Cos(AccumulatedTime * 1.0f) * 0.02f, 0.0f, 1.0f);
}

void UEmotionCognitionComponent::TriggerMemoryEcho(const FString& MemoryID)
{
    // Find the memory and reinforce its emotional impact
    for (FCognitiveMemoryNode& Memory : EmotionalMemoryBank)
    {
        if (Memory.MemoryID == MemoryID)
        {
            // Reinforce current emotional state based on memory's charge
            RegisterEmotion(
                Memory.EmotionalCharge * ReinforcementFactor * (Memory.HapticContext.ResultingValence > 0 ? 1.0f : -1.0f),
                Memory.EmotionalCharge * ReinforcementFactor,
                Memory.EmotionalCharge * ReinforcementFactor
            );
            Memory.DecayProgress = FMath::Max(0.0f, Memory.DecayProgress - ReinforcementFactor); // Reduce decay
            UE_LOG(LogTemp, Log, TEXT("[EmotionMind] Triggered memory echo for '%s'. Reinforced emotions."), *MemoryID);
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("[EmotionMind] Memory '%s' not found for echoing."), *MemoryID);
}

float UEmotionCognitionComponent::CalculatePulseRate() const
{
    // Conceptual pulse rate: higher with arousal, modulated by valence
    float BaseRate = 0.5f; // Normalized base
    float ArousalEffect = Arousal * 0.4f; // Arousal increases pulse
    float ValenceEffect = Valence * 0.1f; // Positive valence slightly increases, negative slightly decreases

    return FMath::Clamp(BaseRate + ArousalEffect + ValenceEffect, 0.0f, 1.0f);
}

void UEmotionCognitionComponent::ModulateEmotionFromHaptic(float Intensity, FString RegionTag)
{
    float Sensitivity = HapticSensitivityByRegion.Contains(RegionTag) ? HapticSensitivityByRegion[RegionTag] : 0.1f;
    float ModulatedIntensity = Intensity * Sensitivity;

    // Example modulation logic:
    // Positive touch (e.g., "Spine", "Face") increases valence and arousal
    // Neutral touch might only affect arousal slightly
    // Negative (e.g., painful, or contextually negative) could decrease valence

    float ValenceMod = 0.0f;
    float ArousalMod = ModulatedIntensity; // All touch increases arousal to some degree

    if (RegionTag == TEXT("Spine") || RegionTag == TEXT("Face") || RegionTag == TEXT("Pelvis"))
    {
        ValenceMod = ModulatedIntensity; // Positive valence for sensitive regions
    }
    else
    {
        ValenceMod = -ModulatedIntensity * 0.1f; // Slight negative for less pleasant/unfamiliar regions
    }

    RegisterEmotion(ValenceMod, ArousalMod, ModulatedIntensity);
    UE_LOG(LogTemp, Log, TEXT("[EmotionMind] Haptic input from '%s' (Intensity: %.2f) modulated emotions. (V:%.2f A:%.2f)"), *RegionTag, Intensity, ValenceMod, ArousalMod);
}

void UEmotionCognitionComponent::StoreHapticEmotionMemory(const FAetherTouchPacket& Packet)
{
    FCognitiveMemoryNode NewMemory;
    NewMemory.MemoryID = FString::Printf(TEXT("HapticMemory_%s_%s"), *Packet.RegionTag, *FDateTime::UtcNow().ToString());
    NewMemory.EmotionalCharge = (FMath::Abs(Valence) + Arousal) / 2.0f; // Average of absolute valence and arousal
    NewMemory.VolitionTension = 0.0f; // Placeholder for now
    NewMemory.DecayProgress = 0.0f; // Fresh memory

    NewMemory.HapticContext.RegionTag = Packet.RegionTag;
    NewMemory.HapticContext.TouchIntensity = Packet.Intensity;
    NewMemory.HapticContext.ResultingValence = Valence; // Capture current state after modulation
    NewMemory.HapticContext.ResultingArousal = Arousal; // Capture current state after modulation
    NewMemory.HapticContext.Timestamp = FDateTime::UtcNow();

    EmotionalMemoryBank.Add(NewMemory);
    UE_LOG(LogTemp, Log, TEXT("[EmotionMind] Stored Haptic Emotion Memory: ID='%s', Charge=%.2f, Region='%s'"), *NewMemory.MemoryID, NewMemory.EmotionalCharge, *Packet.RegionTag);
}

void UEmotionCognitionComponent::UpdateEmotionalOscillators(float DeltaTime)
{
    // Placeholder for more complex, organic emotional oscillations
    // For now, simple sine/cos waves are in GetCurrentValence/Arousal.
}

void UEmotionCognitionComponent::DecayEmotionalMemory(float DeltaTime)
{
    for (int32 i = EmotionalMemoryBank.Num() - 1; i >= 0; --i)
    {
        EmotionalMemoryBank[i].DecayProgress += EmotionalDecayRate * DeltaTime;
        if (EmotionalMemoryBank[i].DecayProgress >= 1.0f)
        {
            UE_LOG(LogTemp, Log, TEXT("[EmotionMind] Memory '%s' decayed and removed."), *EmotionalMemoryBank[i].MemoryID);
            EmotionalMemoryBank.RemoveAt(i);
        }
    }
}
