#include "EnvironmentalResonator.h"

UEnvironmentalResonator::UEnvironmentalResonator()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UEnvironmentalResonator::BeginPlay()
{
    Super::BeginPlay();
}
float UEnvironmentalResonator::GetEnvironmentalInfluence(const FString& StimulusType, float Intensity)
{
    UE_LOG(LogTemp, Log, TEXT("[Environment] Calculating influence for Stimulus '%s' (Intensity: %.2f)"), *StimulusType, Intensity);
    return Intensity * 0.1f; // Simple simulated influence
}
