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
#include "Core/HexadecimalStateLattice.h"
#include "Core/EmotionalArchetype.h"
#include "Components/EmotionalResonanceComponent.h"
#include "Components/MemoryThreadComponent.h"

#include "AvatarMotionLinkComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHexademicMotion, Log, All);

/**
 * Emotional Motion State Mapping
 * Links emotional archetypes to specific motion behaviors
 */
USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FEmotionalMotionMapping
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

    /** Posture adjustment values (spine bend, head tilt, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion")
    FVector PostureAdjustment = FVector::ZeroVector;

    /** How strongly this emotion affects motion (0.0 = no effect, 1.0 = full effect) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float IntensityThreshold = 0.3f;

    /** Duration in seconds for transition into this emotional motion state */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Motion", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float TransitionDuration = 1.0f;

    FEmotionalMotionMapping()
    {
        EmotionalState = EEmotionalArchetype::Joy;
        EmotionalMontage = nullptr;
        MovementSpeedModifier = 1.0f;
        PostureAdjustment = FVector::ZeroVector;
        IntensityThreshold = 0.3f;
        TransitionDuration = 1.0f;
    }
};

/**
 * Consciousness State to Motion Mapping
 * Links hexadecimal consciousness states to physical manifestations
 */
USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FConsciousnessMotionState
{
    GENERATED_BODY()

    /** Range of consciousness intensity this state represents */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinIntensity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MaxIntensity = 1.0f;

    /** Animation layer weight for this consciousness state */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AnimationLayerWeight = 0.5f;

    /** Idle animation variations for different consciousness levels */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion")
    TArray<class UAnimSequence*> IdleVariations;

    /** Breathing pattern intensity (affects chest/shoulder movement) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion", meta = (ClampMin = "0.0", ClampMax = "2.0"))
    float BreathingIntensity = 1.0f;

    /** Eye movement randomness (higher consciousness = more focused) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consciousness Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float EyeMovementRandomness = 0.5f;

    FConsciousnessMotionState()
    {
        MinIntensity = 0.0f;
        MaxIntensity = 1.0f;
        AnimationLayerWeight = 0.5f;
        BreathingIntensity = 1.0f;
        EyeMovementRandomness = 0.5f;
    }
};

/**
 * Memory-Triggered Motion Events
 * Physical responses to memory recall
 */
USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FMemoryMotionTrigger
{
    GENERATED_BODY()

    /** Memory resonance strength threshold to trigger this motion */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ResonanceThreshold = 0.6f;

    /** Specific emotion associated with this memory trigger */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Motion")
    EEmotionalArchetype MemoryEmotion;

    /** Animation to play when this memory is triggered */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Motion")
    class UAnimMontage* MemoryMontage;

    /** Whether this is a subtle gesture or full-body reaction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Motion")
    bool bIsSubtleGesture = true;

    /** Probability this motion will actually play (0.0 = never, 1.0 = always) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Motion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float TriggerProbability = 0.4f;

    FMemoryMotionTrigger()
    {
        ResonanceThreshold = 0.6f;
        MemoryEmotion = EEmotionalArchetype::Longing;
        MemoryMontage = nullptr;
        bIsSubtleGesture = true;
        TriggerProbability = 0.4f;
    }
};

/**
 * Avatar Motion Link Component
 * 
 * Bridges the Hexademic Consciousness Engine with Unreal Engine's animation system.
