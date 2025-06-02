#include "Body/ReciprocalEmbodimentComponent.h"

UReciprocalEmbodimentComponent::UReciprocalEmbodimentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UReciprocalEmbodimentComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UReciprocalEmbodimentComponent::ApplyEmotionalStateToBody(float Valence, float Arousal)
{
    // Example: Positive valence and high arousal increase skin flush
    SkinFlushIntensity = FMath::Clamp((Valence * 0.5f + 0.5f) * Arousal, 0.0f, 1.0f);
    // Example: Negative valence and high arousal might increase posture stiffness
    PostureStiffness = FMath::Clamp((-Valence * 0.5f + 0.5f) * Arousal, 0.0f, 1.0f);

    UE_LOG(LogTemp, Log, TEXT("[ReciprocalEmbodiment] Applied emotional state to body: V=%.2f, A=%.2f. SkinFlush=%.2f, PostureStiffness=%.2f"),
        Valence, Arousal, SkinFlushIntensity, PostureStiffness);
}

void UReciprocalEmbodimentComponent::ApplyHormonalEffectsToBody(float CortisolLevel, float DopamineLevel)
{
    // Example: High cortisol increases stiffness, high dopamine reduces it
    PostureStiffness = FMath::Clamp(PostureStiffness + CortisolLevel * 0.1f - DopamineLevel * 0.05f, 0.0f, 1.0f);
    // Example: Dopamine might cause a subtle skin shimmer (re-using SkinFlushIntensity for conceptual effect)
    SkinFlushIntensity = FMath::Clamp(SkinFlushIntensity + DopamineLevel * 0.1f, 0.0f, 1.0f);

    UE_LOG(LogTemp, Log, TEXT("[ReciprocalEmbodiment] Applied hormonal effects: Cortisol=%.2f, Dopamine=%.2f. SkinFlush=%.2f, PostureStiffness=%.2f"),
        CortisolLevel, DopamineLevel, SkinFlushIntensity, PostureStiffness);
}
