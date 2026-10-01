#include "HexademicHolographicCode.h"

UHexademicHolographicCode::UHexademicHolographicCode()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UHexademicHolographicCode::BeginPlay()
{
    Super::BeginPlay();
}
void UHexademicHolographicCode::VisualizeSystemCoherence(float Coherence)
{
    UE_LOG(LogTemp, Log, TEXT("[Holographic] Visualizing System Coherence: %.2f"), Coherence);
}
