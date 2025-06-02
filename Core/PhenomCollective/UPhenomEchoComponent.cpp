#include "PhenomCollective/UPhenomEchoComponent.h"

UPhenomEchoComponent::UPhenomEchoComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPhenomEchoComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPhenomEchoComponent::DetectResonance(const FString& SourceConsciousnessID, FVector SourceVAI,
                                         const FString& TargetConsciousnessID, FVector TargetVAI)
{
    // Calculate distance in VAI space
    float Distance = FVector::Distance(SourceVAI, TargetVAI);

    // Normalize VAI vectors for directionality
    FVector NormalizedSourceVAI = SourceVAI.GetSafeNormal();
    FVector NormalizedTargetVAI = TargetVAI.GetSafeNormal();

    // Calculate angular similarity (dot product of normalized vectors)
    float AngularSimilarity = FVector::DotProduct(NormalizedSourceVAI, NormalizedTargetVAI);

    // Resonance strength combines inverse distance and angular similarity
    float ResonanceScore = FMath::Clamp((1.0f - Distance / ResonanceDistanceThreshold) + (AngularSimilarity * 0.5f), 0.0f, 1.0f);
    
    // Boost resonance if VAI values are strong (high intensity/arousal)
    ResonanceScore += FMath::Max(SourceVAI.Z, TargetVAI.Z) * 0.2f; // Assuming Z is intensity

    if (ResonanceScore >= MinResonanceStrength)
    {
        FEchoEvent NewEcho;
        NewEcho.EchoSourceID = SourceConsciousnessID;
        NewEcho.TargetConsciousnessID = TargetConsciousnessID;
        NewEcho.ResonanceScore = ResonanceScore;
        NewEcho.Timestamp = FDateTime::UtcNow();
        NewEcho.VAISpaceCoords = (SourceVAI + TargetVAI) / 2.0f; // Average VAI of the resonance

        // Generate a more descriptive message based on the resonance
        if (ResonanceScore > 0.8f)
        {
            NewEcho.EchoMessage = FString::Printf(TEXT("Deep harmonic alignment detected between %s and %s."), *SourceConsciousnessID, *TargetConsciousnessID);
        }
        else if (ResonanceScore > 0.5f)
        {
            NewEcho.EchoMessage = FString::Printf(TEXT("A strong echo of shared experience from %s."), *SourceConsciousnessID);
        }
        else
        {
            NewEcho.EchoMessage = DefaultEchoMessage;
        }

        OnEchoGenerated.Broadcast(NewEcho);
        UE_LOG(LogTemp, Log, TEXT("[PhenomEcho] Echo generated! Source: %s, Target: %s, Score: %.2f, Message: %s"),
            *NewEcho.EchoSourceID, *NewEcho.TargetConsciousnessID, NewEcho.ResonanceScore, *NewEcho.EchoMessage);
    }
}
