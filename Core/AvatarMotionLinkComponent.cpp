#include "AvatarMotionLinkComponent.h"
#include "EmotionCognitionComponent.h" // For accessing emotional state
#include "Components/SkeletalMeshComponent.h" // For animating the mesh

UAvatarMotionLinkComponent::UAvatarMotionLinkComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAvatarMotionLinkComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAvatarMotionLinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!EmotionMind || !TargetMesh) return;

    const float Valence = EmotionMind->GetCurrentValence(); // [-1, 1]
    const float Arousal = EmotionMind->GetCurrentArousal(); // [0, 1]

    // --- Breathing Modulation ---
    // Higher arousal -> faster, deeper breath (conceptual, assumes a 'Breath' morph target)
    const float BreathScale = FMath::Lerp(0.8f, MaxBreathExpansion, Arousal);
    // This assumes a morph target named "Breath" on your Metahuman mesh
    TargetMesh->SetMorphTarget(FName("Breath"), BreathScale);
    // --- Posture Adjustment (Head Tilt Example) ---
    // Valence affects spine curvature or head tilt: down when negative, up when positive
    // Maps Valence [-1, 1] to a rotation range
    float TargetTiltDegrees = FMath::Lerp(-HeadTiltIntensity, HeadTiltIntensity, (Valence + 1.0f) / 2.0f);
    FRotator TargetHeadRotation = FRotator(0.0f, 0.0f, TargetTiltDegrees); // Adjust Z for tilt

    // Get current head bone rotation in Component Space
    FTransform HeadTransform = TargetMesh->GetBoneTransform(FName("head"), EBoneSpaces::ComponentSpace);
    FRotator CurrentHeadRotation = HeadTransform.Rotator();

    // Lerp towards the target rotation for smooth transition
    FRotator NewHeadRotation = FMath::RLerp(CurrentHeadRotation, TargetHeadRotation, DeltaTime * PostureAdjustmentSpeed, true);
    // Apply the new rotation to the head bone.
    // This assumes "head" is the bone name. Adjust for your specific skeletal hierarchy.
    TargetMesh->SetBoneRotationByName(FName("head"), NewHeadRotation, EBoneSpaces::ComponentSpace);
    // Further integrations would include:
    // - Full body posture adjustments based on Valence/Arousal/Dominance
    // - Gesture generation based on FCognitiveMemoryNode or specific emotional events
    // - Walk cycle modulation
}

void UAvatarMotionLinkComponent::SetEmotionMind(UEmotionCognitionComponent* InMind)
{
    EmotionMind = InMind;
}

void UAvatarMotionLinkComponent::SetTargetMesh(USkeletalMeshComponent* InMesh)
{
    TargetMesh = InMesh;
}
