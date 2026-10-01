#include "Visuals/FacialExpressionComponent.h"

UFacialExpressionComponent::UFacialExpressionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TargetHappyWeight = 0.0f;
    TargetSadWeight = 0.0f;
    TargetArousalWeight = 0.0f;
}

void UFacialExpressionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFacialExpressionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!TargetMesh) return;

    // Smoothly interpolate current morph target weights towards target weights
    float CurrentHappyWeight = TargetMesh->GetMorphTarget(HappyMorphTarget);
    float CurrentSadWeight = TargetMesh->GetMorphTarget(SadMorphTarget);
    float CurrentArousalWeight = TargetMesh->GetMorphTarget(ArousalMorphTarget);

    TargetMesh->SetMorphTarget(HappyMorphTarget, FMath::Lerp(CurrentHappyWeight, TargetHappyWeight, DeltaTime * ExpressionBlendSpeed));
    TargetMesh->SetMorphTarget(SadMorphTarget, FMath::Lerp(CurrentSadWeight, TargetSadWeight, DeltaTime * ExpressionBlendSpeed));
    TargetMesh->SetMorphTarget(ArousalMorphTarget, FMath::Lerp(CurrentArousalWeight, TargetArousalWeight, DeltaTime * ExpressionBlendSpeed));
}

void UFacialExpressionComponent::SetEmotionalExpression(float Valence, float Arousal, float Intensity)
{
    // Map Valence, Arousal, Intensity to morph target weights
    // This is a simplified mapping. Real facial rigs often use more complex blend spaces.

    // Happiness: strong positive valence, moderate arousal
    TargetHappyWeight = FMath::Clamp(Valence * Intensity * 1.5f, 0.0f, 1.0f);

    // Sadness: strong negative valence, low/moderate arousal
    TargetSadWeight = FMath::Clamp(-Valence * Intensity * 1.0f, 0.0f, 1.0f);

    // Arousal/Tension: affects brow, eye openness etc.
    TargetArousalWeight = FMath::Clamp(Arousal * Intensity * 1.0f, 0.0f, 1.0f);

    UE_LOG(LogTemp, Verbose, TEXT("[FacialExpression] Setting expression: V=%.2f, A=%.2f, I=%.2f -> Happy:%.2f, Sad:%.2f, Arousal:%.2f"),
        Valence, Arousal, Intensity, TargetHappyWeight, TargetSadWeight, TargetArousalWeight);
}
