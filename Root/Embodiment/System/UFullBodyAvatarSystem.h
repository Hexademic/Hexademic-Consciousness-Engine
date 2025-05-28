#include "UFullBodyAvatarSystem.h"

UFullBodyAvatarSystem::UFullBodyAvatarSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFullBodyAvatarSystem::BeginPlay()
{
    Super::BeginPlay();
}

void UFullBodyAvatarSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdatePhysiologyInternal();
}

FVector UFullBodyAvatarSystem::GetEmotionalVector() const
{
    return FVector(
        PhysiologyParams.EndorphinLevel,
        PhysiologyParams.ArousalLevel,
        1.0f - PhysiologyParams.CortisolLevel
    );
}

float UFullBodyAvatarSystem::GetCoherenceIndex() const
{
    float coherence = (PhysiologyParams.EndorphinLevel + PhysiologyParams.ArousalLevel) * 0.5f;
    return FMath::Clamp(coherence, 0.0f, 1.0f);
}

void UFullBodyAvatarSystem::BroadcastPhysiologyUpdate()
{
    FPhysiologySnapshot snapshot = PhysiologyParams;
    snapshot.Timestamp = GetWorld()->GetTimeSeconds();
    OnPhysiologyUpdate.Broadcast(snapshot);
}

void UFullBodyAvatarSystem::BroadcastEmotionalStateChange(const FString& Emotion, float Intensity)
{
    OnEmotionalStateChange.Broadcast(Emotion, Intensity);
}

void UFullBodyAvatarSystem::UpdatePhysiologyInternal()
{
    // Replace with your real physiology logic
    PhysiologyParams.HeartRate += FMath::Sin(GetWorld()->TimeSeconds * 0.5f) * 0.2f;
    BroadcastPhysiologyUpdate();
}

void UFullBodyAvatarSystem::UpdateEmotionalStateInternal(const FString& EmotionType, float NewIntensity)
{
    // Example stub
    BroadcastEmotionalStateChange(EmotionType, NewIntensity);
}
