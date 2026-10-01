// AvatarMotionLinkComponent.h - Extended Header
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

// Hexademic Consciousness Engine Includes
#include "EmotionCognitionComponent.h"
#include "Core/HexadecimalStateLattice.h"
#include "Core/EmotionalArchetype.h"
#include "Components/MemoryThreadComponent.h"

#include "AvatarMotionLinkComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHexademicMotion, Log, All);

/**
 * Emotional Motion State Mapping
 * Links emotional archetypes to specific motion behaviors
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FEmotionalMotionMapping
{
    GENERATED_BODY()

    /** The emotional archetype this mapping represents */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    EEmotionalArchetype EmotionalState;

    /** Animation montage to play for this emotional state */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    class UAnimMontage* EmotionalMontage;

    /** Movement speed modifier for this emotional state */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion", meta = (ClampMin = "0.1", ClampMax = "3.0"))
    float MovementSpeedModifier = 1.0f;

    /** Breathing pattern adjustments */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    FVector2D BreathingRange = FVector2D(0.8f, 1.5f); // Min/Max breathing intensity

    /** Posture adjustment values (spine bend, head tilt, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    FRotator PostureAdjustment = FRotator::ZeroRotator;

    /** How strongly this emotion affects motion (0.0 = no effect, 1.0 = full effect) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float IntensityThreshold = 0.3f;

    FEmotionalMotionMapping()
    {
        EmotionalState = EEmotionalArchetype::Joy;
        EmotionalMontage = nullptr;
        MovementSpeedModifier = 1.0f;
        BreathingRange = FVector2D(0.8f, 1.5f);
        PostureAdjustment = FRotator::ZeroRotator;
        IntensityThreshold = 0.3f;
    }
};

/**
 * Memory-Triggered Gesture Configuration
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FMemoryGestureMapping
{
    GENERATED_BODY()

    /** Memory emotional signature that triggers this gesture */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures")
    EEmotionalArchetype TriggerEmotion;

    /** Animation to play when memory is recalled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures")
    class UAnimMontage* GestureMontage;

    /** Probability this gesture will play when memory is triggered [0.0-1.0] */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float TriggerProbability = 0.3f;

    /** Whether this is a subtle gesture (facial) or full body */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures")
    bool bIsSubtleGesture = true;

    /** Cooldown period before this gesture can trigger again */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures")
    float GestureCooldown = 5.0f;

    FMemoryGestureMapping()
    {
        TriggerEmotion = EEmotionalArchetype::Longing;
        GestureMontage = nullptr;
        TriggerProbability = 0.3f;
        bIsSubtleGesture = true;
        GestureCooldown = 5.0f;
    }
};

/**
 * Avatar Motion Link Component - Extended Implementation
 * 
 * Bridges the Hexademic Consciousness Engine with Unreal Engine's animation system.
 * This component translates consciousness states, emotional valence/arousal, and 
 * memory triggers into physical avatar manifestations.
 */
UCLASS(ClassGroup=(HexademicConsciousness), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UAvatarMotionLinkComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAvatarMotionLinkComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // === EXISTING FUNCTIONALITY (from your code) ===
    
    /** Reference to the emotion cognition component */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness Bridge")
    class UEmotionCognitionComponent* EmotionMind;

    /** Target skeletal mesh to animate */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness Bridge")
    class USkeletalMeshComponent* TargetMesh;

    /** Maximum breathing expansion multiplier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing", meta = (ClampMin = "0.5", ClampMax = "3.0"))
    float MaxBreathExpansion = 1.5f;

    /** Head tilt intensity in degrees */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture", meta = (ClampMin = "0.0", ClampMax = "45.0"))
    float HeadTiltIntensity = 15.0f;

    /** Speed of posture adjustments */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float PostureAdjustmentSpeed = 2.0f;

    // === NEW EXTENDED FUNCTIONALITY ===

    /** Emotional motion mappings for different consciousness states */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    TArray<FEmotionalMotionMapping> EmotionalMappings;

    /** Memory-triggered gesture configurations */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Gestures")
    TArray<FMemoryGestureMapping> MemoryGestures;

    /** Reference to memory thread component for gesture triggers */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness Bridge")
    class UMemoryThreadComponent* MemoryThreads;

    /** Current consciousness intensity level [0.0-1.0] */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    float ConsciousnessIntensity = 0.5f;

    /** Current dominant emotional archetype */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consciousness State")
    EEmotionalArchetype DominantEmotion = EEmotionalArchetype::Curiosity;

    /** Breathing animation parameters */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Breathing")
    FName BreathMorphTargetName = "Breath";

    /** Head bone name for posture adjustments */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
    FName HeadBoneName = "head";

    /** Spine bone names for posture adjustments */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Posture")
    TArray<FName> SpineBoneNames = {"spine_01", "spine_02", "spine_03"};

    /** Gesture cooldown timers */
    UPROPERTY()
    TMap<EEmotionalArchetype, float> GestureCooldowns;

    /** Current active gestures */
    UPROPERTY()
    TArray<class UAnimMontage*> ActiveGestures;

public:
    // === EXISTING METHODS (from your code) ===
    
    UFUNCTION(BlueprintCallable, Category = "Consciousness Bridge")
    void SetEmotionMind(UEmotionCognitionComponent* InMind);

    UFUNCTION(BlueprintCallable, Category = "Consciousness Bridge")
    void SetTargetMesh(USkeletalMeshComponent* InMesh);

    // === NEW EXTENDED METHODS ===

    /** Set memory thread component for gesture triggering */
    UFUNCTION(BlueprintCallable, Category = "Consciousness Bridge")
    void SetMemoryThreadComponent(UMemoryThreadComponent* InMemoryThreads);

    /** Update consciousness state from hexadecimal lattice */
    UFUNCTION(BlueprintCallable, Category = "Consciousness State")
    void UpdateConsciousnessState(const FHexadecimalStateLattice& StateLattice);

    /** Trigger specific emotional state transition */
    UFUNCTION(BlueprintCallable, Category = "Emotional Motion")
    void TriggerEmotionalTransition(EEmotionalArchetype NewEmotion, float TransitionDuration = 1.0f);

    /** Check and trigger memory-based gestures */
    UFUNCTION(BlueprintCallable, Category = "Memory Gestures")
    void ProcessMemoryGestures(float DeltaTime);

    /** Apply full-body posture adjustments based on consciousness state */
    UFUNCTION(BlueprintCallable, Category = "Posture")
    void ApplyConsciousnessPosture(float DeltaTime);

    /** Modulate walk cycle based on emotional state */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ModulateWalkCycle(float DeltaTime);

    /** Get current emotional motion mapping */
    UFUNCTION(BlueprintCallable, Category = "Emotional Motion")
    FEmotionalMotionMapping GetCurrentEmotionalMapping() const;

    /** Blueprint event for consciousness state changes */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consciousness Events")
    void OnConsciousnessStateChanged(float NewIntensity, EEmotionalArchetype NewDominantEmotion);

    /** Blueprint event for memory gesture triggers */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consciousness Events")
    void OnMemoryGestureTriggered(EEmotionalArchetype TriggerEmotion, class UAnimMontage* GestureMontage);

    /** Blueprint event for emotional transition start */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consciousness Events")
    void OnEmotionalTransitionBegin(EEmotionalArchetype FromEmotion, EEmotionalArchetype ToEmotion);

protected:
    /** Process breathing modulation (enhanced version) */
    void ProcessBreathingModulation(float DeltaTime, float Valence, float Arousal);

    /** Process posture adjustment (enhanced version) */
    void ProcessPostureAdjustment(float DeltaTime, float Valence, float Arousal);

    /** Process emotional animation blending */
    void ProcessEmotionalAnimation(float DeltaTime);

    /** Check for memory-triggered gestures */
    void CheckMemoryTriggers(float DeltaTime);

    /** Update gesture cooldowns */
    void UpdateGestureCooldowns(float DeltaTime);

    /** Calculate consciousness intensity from valence/arousal */
    float CalculateConsciousnessIntensity(float Valence, float Arousal) const;

    /** Determine dominant emotion from current state */
    EEmotionalArchetype DetermineDominantEmotion(float Valence, float Arousal) const;

private:
    /** Previous consciousness state for change detection */
    float PreviousConsciousnessIntensity = 0.5f;
    EEmotionalArchetype PreviousDominantEmotion = EEmotionalArchetype::Curiosity;

    /** Transition timers */
    float EmotionalTransitionTimer = 0.0f;
    float TransitionDuration = 1.0f;
    EEmotionalArchetype TransitionFromEmotion = EEmotionalArchetype::Curiosity;
    EEmotionalArchetype TransitionToEmotion = EEmotionalArchetype::Curiosity;

    /** Animation instance cache */
    UPROPERTY()
    class UAnimInstance* CachedAnimInstance;
};

// ===============================================================================
// IMPLEMENTATION FILE - Extended AvatarMotionLinkComponent.cpp
// ===============================================================================

/* 
Enhanced TickComponent implementation with full consciousness bridge functionality:
*/

void UAvatarMotionLinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!EmotionMind || !TargetMesh) return;

    // Get current emotional state
    const float Valence = EmotionMind->GetCurrentValence(); // [-1, 1]
    const float Arousal = EmotionMind->GetCurrentArousal(); // [0, 1]

    // Update consciousness state
    ConsciousnessIntensity = CalculateConsciousnessIntensity(Valence, Arousal);
    EEmotionalArchetype NewDominantEmotion = DetermineDominantEmotion(Valence, Arousal);

    // Check for consciousness state changes
    if (FMath::Abs(ConsciousnessIntensity - PreviousConsciousnessIntensity) > 0.1f || 
        NewDominantEmotion != PreviousDominantEmotion)
    {
        OnConsciousnessStateChanged(ConsciousnessIntensity, NewDominantEmotion);
        PreviousConsciousnessIntensity = ConsciousnessIntensity;
        PreviousDominantEmotion = NewDominantEmotion;
    }

    DominantEmotion = NewDominantEmotion;

    // === ORIGINAL FUNCTIONALITY (Enhanced) ===
    ProcessBreathingModulation(DeltaTime, Valence, Arousal);
    ProcessPostureAdjustment(DeltaTime, Valence, Arousal);

    // === NEW EXTENDED FUNCTIONALITY ===
    ProcessEmotionalAnimation(DeltaTime);
    ProcessMemoryGestures(DeltaTime);
    ApplyConsciousnessPosture(DeltaTime);
    ModulateWalkCycle(DeltaTime);
    UpdateGestureCooldowns(DeltaTime);
}

void UAvatarMotionLinkComponent::ProcessBreathingModulation(float DeltaTime, float Valence, float Arousal)
{
    // Enhanced breathing with emotional mapping
    FEmotionalMotionMapping CurrentMapping = GetCurrentEmotionalMapping();
    
    // Base breathing from arousal
    float BaseBreathScale = FMath::Lerp(0.8f, MaxBreathExpansion, Arousal);
    
    // Modify with emotional mapping
    float EmotionalBreathMin = CurrentMapping.BreathingRange.X;
    float EmotionalBreathMax = CurrentMapping.BreathingRange.Y;
    float FinalBreathScale = FMath::Lerp(EmotionalBreathMin, EmotionalBreathMax, Arousal);
    
    // Apply consciousness intensity modifier
    FinalBreathScale *= FMath::Lerp(0.8f, 1.2f, ConsciousnessIntensity);
    
    // Smooth breathing animation
    float BreathCycle = FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f * PI * FinalBreathScale) * 0.5f + 0.5f;
    TargetMesh->SetMorphTarget(BreathMorphTargetName, BreathCycle * FinalBreathScale);
}

void UAvatarMotionLinkComponent::ProcessEmotionalAnimation(float DeltaTime)
{
    // Find and blend emotional animations
    FEmotionalMotionMapping CurrentMapping = GetCurrentEmotionalMapping();
    
    if (CurrentMapping.EmotionalMontage && CachedAnimInstance)
    {
        float CurrentIntensity = EmotionMind ? EmotionMind->GetCurrentArousal() : 0.5f;
        
        if (CurrentIntensity > CurrentMapping.IntensityThreshold)
        {
            // Play emotional animation if not already playing
            if (!CachedAnimInstance->Montage_IsPlaying(CurrentMapping.EmotionalMontage))
            {
                CachedAnimInstance->Montage_Play(CurrentMapping.EmotionalMontage, 1.0f);
            }
            
            // Adjust playback rate based on arousal
            float PlayRate = FMath::Lerp(0.8f, 1.5f, CurrentIntensity);
            CachedAnimInstance->Montage_SetPlayRate(CurrentMapping.EmotionalMontage, PlayRate);
        }
    }
}

void UAvatarMotionLinkComponent::ProcessMemoryGestures(float DeltaTime)
{
    if (!MemoryThreads) return;

    // Check for recent memory activations
    for (const FMemoryGestureMapping& GestureMapping : MemoryGestures)
    {
        // Check if this emotion is on cooldown
        if (GestureCooldowns.Contains(GestureMapping.TriggerEmotion))
        {
            continue; // Skip if on cooldown
        }

        // Query memory threads for recent emotional memories
        // This would integrate with your memory system
        // For now, using random trigger based on current emotional state
        if (DominantEmotion == GestureMapping.TriggerEmotion)
        {
            if (FMath::RandRange(0.0f, 1.0f) < GestureMapping.TriggerProbability * DeltaTime)
            {
                // Trigger gesture
                if (GestureMapping.GestureMontage && CachedAnimInstance)
                {
                    CachedAnimInstance->Montage_Play(GestureMapping.GestureMontage, 1.0f);
                    OnMemoryGestureTriggered(GestureMapping.TriggerEmotion, GestureMapping.GestureMontage);
                    
                    // Set cooldown
                    GestureCooldowns.Add(GestureMapping.TriggerEmotion, GestureMapping.GestureCooldown);
                }
            }
        }
    }
}

FEmotionalMotionMapping UAvatarMotionLinkComponent::GetCurrentEmotionalMapping() const
{
    for (const FEmotionalMotionMapping& Mapping : EmotionalMappings)
    {
        if (Mapping.EmotionalState == DominantEmotion)
        {
            return Mapping;
        }
    }
    
    // Return default mapping if none found
    return FEmotionalMotionMapping();
}

float UAvatarMotionLinkComponent::CalculateConsciousnessIntensity(float Valence, float Arousal) const
{
    // Consciousness intensity as combination of emotional activation
    // Higher arousal = higher consciousness, but extreme valence also increases awareness
    float ValenceIntensity = FMath::Abs(Valence); // Distance from neutral
    return FMath::Clamp((Arousal + ValenceIntensity) * 0.5f, 0.0f, 1.0f);
}

EEmotionalArchetype UAvatarMotionLinkComponent::DetermineDominantEmotion(float Valence, float Arousal) const
{
    // Map valence/arousal to emotional archetypes
    // This is a simplified mapping - you might want more sophisticated classification
    
    if (Arousal > 0.7f)
    {
        if (Valence > 0.5f) return EEmotionalArchetype::Joy;
        else if (Valence < -0.5f) return EEmotionalArchetype::Rage;
        else return EEmotionalArchetype::Awe;
    }
    else if (Arousal < 0.3f)
    {
        if (Valence < -0.5f) return EEmotionalArchetype::Grief;
        else return EEmotionalArchetype::Longing;
    }
    else
    {
        if (Valence < -0.3f) return EEmotionalArchetype::Fear;
        else return EEmotionalArchetype::Curiosity;
    }
}
